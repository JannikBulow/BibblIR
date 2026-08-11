// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_INSTRUCTION_FIELD_INSTRUCTION_H
#define BIBBLIR_IR_INSTRUCTION_FIELD_INSTRUCTION_H

#include "BibblIR/ir/instruction.h"

namespace bibblir {
    class GetMemberInstruction : public Instruction {
        friend class CodegenVisitor;
        friend class IRBuilder;
        friend class PrintVisitor;
    public:
        std::vector<std::reference_wrapper<Value*>> getOperands() override;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        Value* mObject;
        Value* mField;

        int mValueId;

        GetMemberInstruction(BasicBlock* parent, Value* object, Value* field);
    };
}

#endif //BIBBLIR_IR_INSTRUCTION_FIELD_INSTRUCTION_H
