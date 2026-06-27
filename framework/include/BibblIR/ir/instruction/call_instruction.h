// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_INSTRUCTION_CALL_INSTRUCTION_H
#define BIBBLIR_IR_INSTRUCTION_CALL_INSTRUCTION_H

#include "BibblIR/ir/instruction.h"

namespace bibblir {
    class CallInstruction : public Instruction {
        friend class CodegenVisitor;
        friend class IRBuilder;
        friend class PrintVisitor;
    public:
        std::vector<std::reference_wrapper<Value*>> getOperands() override;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        Value* mCallee;
        std::vector<Value*> mParameters;
        int mValueId;

        CallInstruction(BasicBlock* parent, Value* callee, std::vector<Value*> parameters);
    };
}

#endif //BIBBLIR_IR_INSTRUCTION_CALL_INSTRUCTION_H
