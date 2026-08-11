// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_INSTRUCTION_NEW_INSTRUCTION_H
#define BIBBLIR_IR_INSTRUCTION_NEW_INSTRUCTION_H

#include "BibblIR/ir/instruction.h"

namespace bibblir {
    class BIBBLIR_EXPORT NewInstruction : public Instruction {
        friend class CodegenVisitor;
        friend class IRBuilder;
        friend class PrintVisitor;
    public:
        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        int mValueId;

        NewInstruction(BasicBlock* parent, Type* type);
    };
}

#endif //BIBBLIR_IR_INSTRUCTION_NEW_INSTRUCTION_H
