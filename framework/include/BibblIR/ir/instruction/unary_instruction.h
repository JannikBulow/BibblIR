// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_INSTRUCTION_UNARY_INSTRUCTION_H
#define BIBBLIR_IR_INSTRUCTION_UNARY_INSTRUCTION_H

#include "BibblIR/ir/instruction.h"

namespace bibblir {
    class BIBBLIR_EXPORT UnaryInstruction : public Instruction {
        friend class CodegenVisitor;
        friend class IRBuilder;
        friend class PrintVisitor;
    public:
        enum Operator {
            NEG,
            ABS,
            NOT
        };

        Operator getOperator() const;

        std::vector<std::reference_wrapper<Value*>> getOperands() override;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        Value* mOperand;
        Operator mOperator;

        int mValueId;

        UnaryInstruction(BasicBlock* parent, Value* operand, Operator op);
    };
}

#endif //BIBBLIR_IR_INSTRUCTION_UNARY_INSTRUCTION_H
