// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/new_instruction.h"

#include "BibblIR/ir/function.h"

#include "BibblIR/visitor/visitor.h"

#include "BibblIR/module.h"

namespace bibblir {
    std::vector<std::reference_wrapper<Value*>> NewInstruction::getOperands() {
        return {mClass};
    }

    std::string NewInstruction::identifier() const {
        return getName(mValueId);
    }

    void NewInstruction::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    NewInstruction::NewInstruction(BasicBlock* parent, Value* clas)
        : Instruction(parent->getModule(), parent)
        , mClass(clas)
        , mValueId(mModule.getNextValueId()) {
        mType = mClass->getType();
    }
}
