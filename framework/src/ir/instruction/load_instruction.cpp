// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/load_instruction.h"

#include "BibblIR/ir/basicblock.h"

#include "BibblIR/visitor/visitor.h"

#include "BibblIR/module.h"

namespace bibblir {
    std::vector<std::reference_wrapper<Value*>> LoadInstruction::getOperands() {
        return {mVariable};
    }

    std::string LoadInstruction::identifier() const {
        return getName(mValueId);
    }

    void LoadInstruction::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    LoadInstruction::LoadInstruction(BasicBlock* parent, Value* variable)
        : Instruction(parent->getModule(), parent)
        , mVariable(variable)
        , mValueId(mModule.getNextValueId()) {
        mType = mVariable->getType();
    }
}
