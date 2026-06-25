// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/phi_instruction.h"

#include "BibblIR/ir/basicblock.h"

#include "BibblIR/visitor/visitor.h"

#include "BibblIR/module.h"

namespace bibblir {
    void PhiInstruction::addIncoming(Value* value, BasicBlock* basicBlock) {
        mIncoming.emplace_back(value, basicBlock);
    }

    void PhiInstruction::eraseFromParent() {
        std::erase(mParent->mPhis, this);
        Instruction::eraseFromParent();
    }

    std::vector<std::reference_wrapper<Value*>> PhiInstruction::getOperands() {
        std::vector<std::reference_wrapper<Value*>> operands;
        for (auto& incoming : mIncoming) {
            operands.emplace_back(incoming.first);
        }
        return operands;
    }

    std::string PhiInstruction::identifier() const {
        return getName(mValueId);
    }

    void PhiInstruction::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    PhiInstruction::PhiInstruction(BasicBlock* parent, Type* type)
        : Instruction(parent->getModule(), parent)
        , mValueId(mModule.getNextValueId()) {
        mParent->mPhis.push_back(this);
        mType = type;
    }

    void PhiInstruction::setEmittedValue() {
        mEmittedValue = mVReg->toOperand();
    }
}
