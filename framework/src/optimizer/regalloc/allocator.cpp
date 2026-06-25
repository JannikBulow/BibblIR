// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/phi_instruction.h"

#include "BibblIR/optimizer/regalloc/allocator.h"

#include <algorithm>
#include <set>

namespace bibblir {
    void RegAlloc::assignVRegs(Function* function) {
        setLiveIntervals(function);
        doRegAlloc(function);
    }

    void RegAlloc::doRegAlloc(Function* function) {
        std::vector<VReg*> availableVRegs;

        int regCount = 0;
        auto createVReg = [function, &regCount]() {
            int index = regCount++;
            function->mVRegs.push_back(std::make_unique<VReg>(index, index));
            function->mVRegs.back()->mUses++;
            return function->mVRegs.back().get();
        };

        auto getNextFreeVReg = [&availableVRegs, &createVReg](const std::vector<VReg*>& disallowed, int preferred) {
            if (availableVRegs.empty()) {
                return createVReg();
            }

            if (preferred != -1) {
                auto it = std::ranges::find_if(availableVRegs, [preferred, &disallowed](const VReg* vreg) {
                    return std::ranges::find(disallowed, vreg) == disallowed.end() && vreg->getActualRegister() == preferred;
                });
                if (it != availableVRegs.end()) {
                    VReg* vreg = *it;
                    availableVRegs.erase(it);
                    vreg->mUses++;
                    return vreg;
                }
            }

            auto it = std::ranges::find_if(availableVRegs, [&disallowed](const VReg* vreg) {
                return std::ranges::find(disallowed, vreg) == disallowed.end();
            });
            if (it == availableVRegs.end()) return createVReg();

            VReg* vreg = *it;
            availableVRegs.erase(it);
            vreg->mUses++;
            return vreg;
        };

        std::vector<Value*> activeValues;
        auto expireOldIntervals = [&activeValues, &availableVRegs](int i) {
            std::erase_if(activeValues, [i, &availableVRegs](Value* value) {
                if (value->mInterval.second <= i) {
                    availableVRegs.push_back(value->mVReg);
                    return true;
                }
                return false;
            });
        };

        struct Compare {
            bool operator()(const Value* lhs, const Value* rhs) const {
                return lhs->mInterval.first < rhs->mInterval.first;
            }
        };

        std::multiset<Value*, Compare> values;
        for (auto& bb : function->basicBlocks()) {
            for (auto& value : bb->mValueList) {
                values.insert(value.get());
            }
        }

        for (auto value : values) {
            expireOldIntervals(value->mId);

            if (value->requiresVReg()) {
                activeValues.push_back(value);
                std::ranges::sort(activeValues, [](Value* lhs, Value* rhs) {
                    return lhs->mInterval.second < rhs->mInterval.second;
                });

                auto disallowed = value->mDisallowedVRegs;
                for (auto id : value->mDisallowedRegisters) {
                    auto it = std::ranges::find_if(function->mVRegs, [id](const auto& vreg) {
                        return vreg->getActualRegister() == id;
                    });
                    if (it != function->mVRegs.end()) {
                        disallowed.push_back(it->get());
                    }
                }

                value->mVReg = getNextFreeVReg(disallowed, value->mPreferredRegister);
            }
        }

        function->mRegisterCount = std::max(1, regCount);
    }

    void RegAlloc::setLiveIntervals(Function* function) {
        int index = 0;

        for (auto& argument : function->mArguments) {
            argument->mInterval.first = 0;
        }

        for (auto& bb : function->basicBlocks()) {
            bb->mInterval.first = index;
            for (auto& value : bb->mValueList) {
                value->mInterval.first = index;
                value->mId = index;
                index++;
            }
            bb->mInterval.second = index;
        }

        for (auto it = function->basicBlocks().rbegin(); it != function->basicBlocks().rend(); ++it) {
            auto& bb = *it;
            std::vector<Value*> live;

            for (auto successor : bb->successors()) {
                std::ranges::copy(successor->liveIn(), std::back_inserter(live));

                for (auto phi : successor->mPhis) {
                    auto incomingIt = std::ranges::find_if(phi->mIncoming, [&bb](const auto& incoming) {
                        return incoming.second == bb.get();
                    });

                    if (incomingIt != phi->mIncoming.end()) {
                        live.push_back(incomingIt->first);
                    }
                }
            }

            for (Value* value : live) {
                value->mInterval.first = std::min(value->mInterval.first, bb->mInterval.first);
                value->mInterval.second = std::max(value->mInterval.second, bb->mInterval.second);
            }

            for (auto valueIt = bb->mValueList.rbegin(); valueIt != bb->mValueList.rend(); ++valueIt) {
                auto& value = *valueIt;

                for (auto operandR : value->getOperands()) {
                    auto operand = operandR.get();
                    operand->mInterval.first = std::min(operand->mInterval.first, bb->mInterval.first);
                    operand->mInterval.second = std::max(operand->mInterval.second, value->mId);
                    live.push_back(operand);
                }
                value->mInterval.first = value->mId;

                std::erase_if(live, [&value](const Value* liveValue) {
                    return liveValue == value.get();
                });
            }

            for (auto phi : bb->mPhis) {
                std::erase(live, phi);
            }

            if (bb->loopEnd()) {
                for (auto value : live) {
                    value->mInterval.first = std::min(value->mInterval.first, bb->mInterval.first);
                    value->mInterval.second = std::max(value->mInterval.second, bb->loopEnd()->mInterval.second);
                }
            }

            bb->liveIn() = live;
        }
    }
}
