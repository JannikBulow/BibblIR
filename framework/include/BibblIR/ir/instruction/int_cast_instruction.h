// Copyright 2026 Jannik Laugmand Bülow

/*
A simple int cast which wraps an existing value with the int type, but with a different int type.
*/

#ifndef BIBBLIR_IR_INSTRUCTION_INT_CAST_INSTRUCTION_H
#define BIBBLIR_IR_INSTRUCTION_INT_CAST_INSTRUCTION_H

#include "BibblIR/ir/instruction.h"

namespace bibblir {
    class IntCastInstruction : public Instruction {
        friend class CodegenVisitor;
        friend class IRBuilder;
        friend class PrintVisitor;
    public:
        Value* getValue() const;

        std::vector<std::reference_wrapper<Value*>> getOperands() override;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        Value* mValue;
        int mValueId;

        IntCastInstruction(BasicBlock* parent, Value* value, Type* destType);
    };
}

#endif //BIBBLIR_IR_INSTRUCTION_INT_CAST_INSTRUCTION_H
