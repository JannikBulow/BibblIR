// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/field_instruction.h"

#include "BibblIR/ir/basicblock.h"

#include "BibblIR/visitor/visitor.h"

#include "BibblIR/module.h"

namespace bibblir {
    std::vector<std::reference_wrapper<Value*>> FieldInstruction::getOperands() {
        return {mObject, mField};
    }

    std::string FieldInstruction::identifier() const {
        return getName(mValueId);
    }

    void FieldInstruction::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    FieldInstruction::FieldInstruction(BasicBlock* parent, Value* object, Value* field)
        : Instruction(parent->getModule(), parent)
        , mObject(object)
        , mField(field)
        , mValueId(mModule.getNextValueId()) {
        mType = mField->getType();
        mRequiresVReg = false;
    }
}
