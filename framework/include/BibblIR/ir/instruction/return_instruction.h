// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_INSTRUCTION_RETURN_INSTRUCTION_H
#define BIBBLIR_IR_INSTRUCTION_RRETURN_INSTRUCTION_H

#include "BibblIR/ir/instruction.h"

namespace bibblir {
    class ReturnInstruction : public Instruction {
        friend class CodegenVisitor;
        friend class IRBuilder;
        friend class PrintVisitor;
    public:
        std::vector<std::reference_wrapper<Value*>> getOperands() override;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        ReturnInstruction(BasicBlock* parent, Value* returnValue);

        Value* mReturnValue;
    };
}

#endif //BIBBLIR_IR_INSTRUCTION_RRETURN_INSTRUCTION_H
