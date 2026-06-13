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

        if (!mReturnValue) {
            // Assumption that r0 will always exist, maybe enforce it in codegen, that way we can avoid allocating another register here
            mRequiresRegister = false;
            assert(mParent->getParent()->getFunctionType()->getReturnType()->isVoidType());
        } else {
            // An ugly hack, but if the return value is a constant, this instruction needs its own register to move it to
            mRequiresRegister = !mReturnValue->requiresRegister();
            assert(mReturnValue->getType() == mParent->getParent()->getFunctionType()->getReturnType());
        }
    }
}
