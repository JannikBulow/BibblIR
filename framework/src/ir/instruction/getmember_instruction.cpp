// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/getmember_instruction.h"

#include "BibblIR/ir/basicblock.h"

#include "BibblIR/visitor/visitor.h"

#include "BibblIR/module.h"

namespace bibblir {
    std::vector<std::reference_wrapper<Value*>> GetMemberInstruction::getOperands() {
        return {mObject, mField};
    }

    std::string GetMemberInstruction::identifier() const {
        return getName(mValueId);
    }

    void GetMemberInstruction::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    GetMemberInstruction::GetMemberInstruction(BasicBlock* parent, Value* object, Value* field)
        : Instruction(parent->getModule(), parent)
        , mObject(object)
        , mField(field)
        , mValueId(mModule.getNextValueId()) {
        mType = mField->getType();
        mRequiresVReg = false;
    }
}
