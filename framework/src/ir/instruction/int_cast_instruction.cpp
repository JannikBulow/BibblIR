// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/int_cast_instruction.h"

#include "BibblIR/ir/basicblock.h"

#include "BibblIR/visitor/visitor.h"

#include "BibblIR/module.h"

#include <cassert>

namespace bibblir {
    Value* IntCastInstruction::getValue() const {
        return mValue;
    }

    std::vector<std::reference_wrapper<Value*>> IntCastInstruction::getOperands() {
        return Instruction::getOperands();
    }

    std::string IntCastInstruction::identifier() const {
        return std::format("{} {}", mType->getName(), getName(mValueId));
    }

    void IntCastInstruction::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    IntCastInstruction::IntCastInstruction(BasicBlock* parent, Value* value, Type* destType)
        : Instruction(parent->getModule(), parent)
        , mValue(value)
        , mValueId(mModule.getNextValueId()) {
        assert(destType->isIntegerType());
        assert(mValue->getType()->isIntegerType());
        mType = destType;
    }
}
