// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_BUILDER_H
#define BIBBLIR_IR_BUILDER_H

#include "BibblIR/ir/value.h"

namespace bibblir {
    class BasicBlock;
    class Function;

    class ConstantBoolean;
    class ConstantInt;

    class BinaryInstruction;
    class ReturnInstruction;

    class BIBBLIR_EXPORT IRBuilder {
    public:
        BasicBlock* getInsertPoint() const;
        void setInsertPoint(BasicBlock* newInsertPoint);

        void insertAfter(Value* value);

        ConstantBoolean* createConstantBoolean(bool value);
        ConstantInt* createConstantInt(intmax_t value, Type* type);

        BinaryInstruction* createBinaryInstruction(Value* left, BinaryInstruction::Operator op, Value* right);

        ReturnInstruction* createReturn(Value* returnValue);

    private:
        BasicBlock* mInsertPoint = nullptr;
        Value* mInsertAfter = nullptr;
    };
}

#endif //BIBBLIR_IR_BUILDER_H
