// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_INSTRUCTION_STORE_INSTRUCTION_H
#define BIBBLIR_IR_INSTRUCTION_STORE_INSTRUCTION_H

#include "BibblIR/ir/instruction.h"

namespace bibblir {
    class StoreInstruction : public Instruction {
        friend class CodegenVisitor;
        friend class IRBuilder;
        friend class PrintVisitor;
    public:
        Value* getVariable() const;
        Value* getValue() const;

        std::vector<std::reference_wrapper<Value*>> getOperands() override;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        Value* mVariable;
        Value* mValue;

        StoreInstruction(BasicBlock* parent, Value* variable, Value* value);
    };
}

#endif //BIBBLIR_IR_INSTRUCTION_STORE_INSTRUCTION_H
