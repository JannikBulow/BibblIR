// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_INSTRUCTION_PHI_INSTRUCTION_H
#define BIBBLIR_IR_INSTRUCTION_PHI_INSTRUCTION_H

#include "BibblIR/ir/instruction.h"

namespace bibblir {
    class BIBBLIR_EXPORT PhiInstruction : public Instruction {
        friend class BasicBlock;
        friend class CodegenVisitor;
        friend class Function;
        friend class IRBuilder;
        friend class PrintVisitor;
        friend class RegAlloc;
    public:
        void addIncoming(Value* value, BasicBlock* basicBlock);

        void eraseFromParent() override;

        std::vector<std::reference_wrapper<Value*>> getOperands() override;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        std::vector<std::pair<Value*, BasicBlock*>> mIncoming;
        int mValueId;

        PhiInstruction(BasicBlock* parent, Type* type);

        void setEmittedValue();
    };
}

#endif //BIBBLIR_IR_INSTRUCTION_PHI_INSTRUCTION_H
