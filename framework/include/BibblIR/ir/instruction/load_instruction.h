// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_INSTRUCTION_LOAD_INSTRUCTION_H
#define BIBBLIR_IR_INSTRUCTION_LOAD_INSTRUCTION_H 1

#include "BibblIR/ir/instruction.h"

namespace bibblir {
    class LoadInstruction : public Instruction {
        friend class CodegenVisitor;
        friend class IRBuilder;
        friend class PrintVisitor;
    public:
        std::vector<std::reference_wrapper<Value*>> getOperands() override;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        Value* mVariable;
        int mValueId;

        LoadInstruction(BasicBlock* parent, Value* variable);
    };
}

#endif // BIBBLIR_IR_INSTRUCTION_LOAD_INSTRUCTION_H
