// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/getmember_instruction.h"
#include "BibblIR/ir/instruction/store_instruction.h"

#include "BibblIR/ir/basicblock.h"

#include "BibblIR/visitor/visitor.h"

namespace bibblir {
    Value* StoreInstruction::getVariable() const {
        return mVariable;
    }

    Value* StoreInstruction::getValue() const {
        return mValue;
    }

    std::vector<std::reference_wrapper<Value*>> StoreInstruction::getOperands() {
        return {mVariable, mValue};
    }

    std::string StoreInstruction::identifier() const {
        return "%undef";
    }

    void StoreInstruction::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    StoreInstruction::StoreInstruction(BasicBlock* parent, Value* variable, Value* value)
        : Instruction(parent->getModule(), parent)
        , mVariable(variable)
        , mValue(value) {
        mRequiresVReg = false;
    }
}
