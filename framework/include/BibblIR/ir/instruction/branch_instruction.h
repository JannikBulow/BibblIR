// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_INSTRUCTION_BRANCH_INSTRUCTION_H
#define BIBBLIR_IR_INSTRUCTION_BRANCH_INSTRUCTION_H

#include "BibblIR/ir/instruction.h"

namespace bibblir {
    class BIBBLIR_EXPORT BranchInstruction : public Instruction {
        friend class CodegenVisitor;
        friend class IRBuilder;
        friend class PrintVisitor;
    public:
        std::vector<std::reference_wrapper<Value*>> getOperands() override;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        Value* mCondition;
        BasicBlock* mTrueBranch;
        BasicBlock* mFalseBranch;

        BranchInstruction(BasicBlock* parent, BasicBlock* destination);
        BranchInstruction(BasicBlock* parent, Value* condition, BasicBlock* trueBranch, BasicBlock* falseBranch);
    };
}

#endif //BIBBLIR_IR_INSTRUCTION_BRANCH_INSTRUCTION_H
