// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/optimizer/regalloc/allocator.h"

namespace bibblir {
    void RegAlloc::assignVRegs(Function* function) {
        setLiveIntervals(function);
        doRegAlloc(function);
    }

    void RegAlloc::doRegAlloc(Function* function) {
        int regCount = 0;
        auto createVReg = [function, &regCount]() {
            int index = regCount++;
            function->mVRegs.push_back(std::make_unique<VReg>(index, index));
            function->mVRegs.back()->mUses++;
            return function->mVRegs.back().get();
        };

        auto getNextFreeVReg = [function, &createVReg](const std::vector<VReg*>& disallowed, int preferred) {
            return createVReg();
        };

        for (const BasicBlockPtr& bb : function->basicBlocks()) {
            for (const ValuePtr& value : bb->mValueList) {
                if (value->mRequiresVReg) {
                    value->mVReg = getNextFreeVReg(value->mDisallowedVRegs, value->mPreferredRegister);
                }
            }
        }

        function->mRegisterCount = std::min(1, regCount);
    }

    void RegAlloc::setLiveIntervals(Function* function) {

    }
}
