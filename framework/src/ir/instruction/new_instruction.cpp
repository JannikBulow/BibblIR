// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/new_instruction.h"

#include "BibblIR/ir/function.h"

#include "BibblIR/visitor/visitor.h"

#include "BibblIR/module.h"

#include <cassert>

namespace bibblir {
    Value* NewInstruction::getParameter() const {
        return mParameter;
    }

    std::string NewInstruction::identifier() const {
        return getName(mValueId);
    }

    void NewInstruction::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    NewInstruction::NewInstruction(BasicBlock* parent, Type* type, Value* parameter)
        : Instruction(parent->getModule(), parent)
        , mParameter(parameter)
        , mValueId(mModule.getNextValueId()) {
        mType = type;

        if (type->isArrayType()) {
            assert(parameter->getType()->isIntegerType());
        }
    }
}
