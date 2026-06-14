// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/return_instruction.h"

#include "BibblIR/ir/basicblock.h"
#include "BibblIR/ir/function.h"

#include "BibblIR/visitor/visitor.h"

#include <cassert>

namespace bibblir {
    std::vector<std::reference_wrapper<Value*>> ReturnInstruction::getOperands() {
        if (mReturnValue) return {mReturnValue};
        else return {};
    }

    std::string ReturnInstruction::identifier() const {
        return "%undef";
    }

    void ReturnInstruction::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    ReturnInstruction::ReturnInstruction(BasicBlock* parent, Value* returnValue)
        : Instruction(parent->getModule(), parent)
        , mReturnValue(returnValue) {
        mRequiresVReg = false;

        if (!mReturnValue) {
            assert(mParent->getParent()->getFunctionType()->getReturnType()->isVoidType());
        } else {
            assert(mReturnValue->getType() == mParent->getParent()->getFunctionType()->getReturnType());
        }
    }
}
