// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/phi_instruction.h"

#include "BibblIR/optimizer/regalloc/allocator.h"

#include <algorithm>
#include <cassert>
#include <numeric>
#include <set>

namespace bibblir {
    void RegAlloc::assignVRegs(Function* function) {
        setLiveIntervals(function);
        doRegAlloc(function);
    }

    void RegAlloc::doRegAlloc(Function* function) {
        std::vector<VReg*> availableVRegs;
        int regCount = 0;

        for (const auto& argument : function->mArguments) {
            function->mVRegs.push_back(std::make_unique<VReg>(regCount++, argument->mIndex));
            availableVRegs.push_back(function->mVRegs.back().get());
        }

        auto createVReg = [function, &regCount]() {
            int index = regCount++;
            function->mVRegs.push_back(std::make_unique<VReg>(index, index));
            function->mVRegs.back()->mUses++;
            return function->mVRegs.back().get();
        };

        auto getNextFreeVReg = [&availableVRegs, &createVReg](int preferred, bool strictPreferred = false) {
            if (availableVRegs.empty()) {
                return createVReg();
            }

            if (preferred != -1) {
                auto it = std::ranges::find_if(availableVRegs, [preferred](const VReg* vreg) {
                    return vreg->getActualRegister() == preferred;
                });

                if (strictPreferred) {
                    assert(it != availableVRegs.end());
                }

                if (it != availableVRegs.end()) {
                    VReg* vreg = *it;
                    availableVRegs.erase(it);
                    vreg->mUses++;
                    return vreg;
                }
            }

            VReg* vreg = availableVRegs.back();
            availableVRegs.pop_back();
            vreg->mUses++;
            return vreg;
        };

        auto getNextFreeVRegRange = [&availableVRegs, &createVReg](uint16_t count) {
            if (!availableVRegs.empty()) {
                std::vector<size_t> order(availableVRegs.size());
                std::iota(order.begin(), order.end(), 0);
                std::ranges::sort(order, [&availableVRegs](size_t lhs, size_t rhs) {
                    return availableVRegs[lhs]->getActualRegister() < availableVRegs[rhs]->getActualRegister();
                });

                int runStart = 0;
                for (int i = 0; i < static_cast<int>(order.size()); ++i) {
                    bool contiguous = i > runStart && availableVRegs[order[i]]->getActualRegister() == availableVRegs[order[i - 1]]->getActualRegister() + 1;

                    if (!contiguous) {
                        runStart = i;
                    }

                    if (i - runStart + 1 == count) {
                        std::vector<VReg*> result;
                        result.reserve(count);
                        std::vector<size_t> toErase(order.begin() + runStart, order.begin() + i + 1);
                        std::ranges::sort(toErase, std::greater<>());
                        for (size_t idx : toErase) {
                            VReg* v = availableVRegs[idx];
                            v->mUses++;
                            result.push_back(v);
                            availableVRegs.erase(availableVRegs.begin() + idx);
                        }
                        std::ranges::reverse(result);
                        return result;
                    }
                }
            }

            std::vector<VReg*> range;
            range.reserve(count);
            for (uint16_t i = 0; i < count; i++) {
                range.push_back(createVReg());
            }
            return range;
        };

        std::vector<Value*> activeValues;
        auto expireOldIntervals = [&activeValues, &availableVRegs](int i) {
            std::erase_if(activeValues, [i, &availableVRegs](Value* value) {
                if (value->mInterval.second <= i) {
                    if (value->mVReg) availableVRegs.push_back(value->mVReg);
                    for (VReg* vreg : value->mVRegRange) {
                        availableVRegs.push_back(vreg);
                    }
                    return true;
                }
                return false;
            });
        };

        for (const auto& argument : function->mArguments) {
            argument->mVReg = getNextFreeVReg(argument->mIndex, true);
            activeValues.push_back(argument.get());
        }

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

                value->mVReg = getNextFreeVReg(value->mPreferredRegister);
            }

            if (value->requiresVRegRange()) {
                activeValues.push_back(value);
                std::ranges::sort(activeValues, [](Value* lhs, Value* rhs) {
                    return lhs->mInterval.second < rhs->mInterval.second;
                });

                value->mVRegRange = getNextFreeVRegRange(value->mVRegRangeSize);
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
