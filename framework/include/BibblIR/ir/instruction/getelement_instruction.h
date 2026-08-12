// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_INSTRUCTION_GETELEMENT_INSTRUCTION_H
#define BIBBLIR_IR_INSTRUCTION_GETELEMENT_INSTRUCTION_H

#include "BibblIR/ir/instruction.h"

namespace bibblir {
    class BIBBLIR_EXPORT GetElementInstruction final : public Instruction {
        friend class CodegenVisitor;
        friend class IRBuilder;
        friend class PrintVisitor;
    public:
        std::vector<std::reference_wrapper<Value*>> getOperands() override;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        Value* mArray;
        Value* mIndex;

        int mValueId;

        GetElementInstruction(BasicBlock* parent, Value* array, Value* index);
    };
}

#endif //BIBBLIR_IR_INSTRUCTION_GETELEMENT_INSTRUCTION_H
