// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/branch_instruction.h"

#include "BibblIR/ir/basicblock.h"

#include "BibblIR/visitor/visitor.h"

#include <cassert>

namespace bibblir {
    std::vector<std::reference_wrapper<Value*>> BranchInstruction::getOperands() {
        if (mCondition) return {mCondition};
        else return {};
    }

    std::string BranchInstruction::identifier() const {
        return "%undef";
    }

    void BranchInstruction::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    BranchInstruction::BranchInstruction(BasicBlock* parent, BasicBlock* destination)
        : Instruction(parent->getModule(), parent)
        , mCondition(nullptr)
        , mTrueBranch(destination)
        , mFalseBranch(nullptr) {
        mRequiresVReg = false;

        parent->successors().push_back(destination);
        destination->predecessors().push_back(parent);
    }

    BranchInstruction::BranchInstruction(BasicBlock* parent, Value* condition, BasicBlock* trueBranch, BasicBlock* falseBranch)
        : Instruction(parent->getModule(), parent)
        , mCondition(condition)
        , mTrueBranch(trueBranch)
        , mFalseBranch(falseBranch) {
        assert(mCondition->getType() == Type::GetBooleanType());

        mRequiresVReg = false;

        parent->successors().push_back(trueBranch);
        parent->successors().push_back(falseBranch);
        trueBranch->predecessors().push_back(parent);
        falseBranch->predecessors().push_back(parent);
    }
}
