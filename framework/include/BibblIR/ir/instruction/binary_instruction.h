// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_INSTRUCTION_BINARY_INSTRUCTION_H
#define BIBBLIR_IR_INSTRUCTION_BINARY_INSTRUCTION_H

#include "BibblIR/ir/instruction.h"

namespace bibblir {
    class BIBBLIR_EXPORT BinaryInstruction : public Instruction {
        friend class CodegenVisitor;
        friend class IRBuilder;
        friend class PrintVisitor;
    public:
        enum Operator {
            ADD, SUB, MUL,
            SDIV, UDIV,
            SMOD, UMOD,

            AND, OR, XOR,

            SHL, SHR, SAR,

            EQ, NE,
            LT, GT,
            LE, GE
        };

        Operator getOperator() const;

        std::vector<std::reference_wrapper<Value*>> getOperands() override;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        Value* mLeft;
        Operator mOperator;
        Value* mRight;

        int mValueId;

        BinaryInstruction(BasicBlock* parent, Value* left, Operator op, Value* right);
    };
}

#endif //BIBBLIR_IR_INSTRUCTION_BINARY_INSTRUCTION_H
