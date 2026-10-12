// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_INSTRUCTION_STRING_INSTRUCTION_H
#define BIBBLIR_IR_INSTRUCTION_STRING_INSTRUCTION_H

#include "BibblIR/ir/instruction.h"

namespace bibblir {
    class BIBBLIR_EXPORT StringInstruction : public Instruction {
        friend class CodegenVisitor;
        friend class IRBuilder;
        friend class PrintVisitor;
    public:
        enum Operator {
            Length,
        };

        Operator getOperator() const;
        Value* getStringOperand() const; // not nullable
        Value* getOtherOperand() const; // nullable

        std::vector<std::reference_wrapper<Value*>> getOperands() override;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        Operator mOperator;
        Value* mStringOperand;
        Value* mOtherOperand;

        int mValueId;

        StringInstruction(BasicBlock* parent, Operator op, Value* stringOperand, Value* otherOperand = nullptr);
    };
}

#endif //BIBBLIR_IR_INSTRUCTION_STRING_INSTRUCTION_H
