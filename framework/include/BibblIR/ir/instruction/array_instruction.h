// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_INSTRUCTION_ARRAY_INSTRUCTION_H
#define BIBBLIR_IR_INSTRUCTION_ARRAY_INSTRUCTION_H

#include "BibblIR/ir/instruction.h"

namespace bibblir {
    class BIBBLIR_EXPORT ArrayInstruction : public Instruction {
        friend class CodegenVisitor;
        friend class IRBuilder;
        friend class PrintVisitor;
    public:
        enum Operator {
            Length,
        };

        Operator getOperator() const;
        Value* getArrayOperand() const; // not nullable
        Value* getOtherOperand() const; // nullable

        std::vector<std::reference_wrapper<Value*>> getOperands() override;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        Operator mOperator;
        Value* mArrayOperand;
        Value* mOtherOperand;

        int mValueId;

        ArrayInstruction(BasicBlock* parent, Operator op, Value* arrayOperand, Value* otherOperand = nullptr);
    };
}

#endif //BIBBLIR_IR_INSTRUCTION_ARRAY_INSTRUCTION_H
