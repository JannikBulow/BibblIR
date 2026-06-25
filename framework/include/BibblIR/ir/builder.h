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
    class PhiInstruction;
    class ReturnInstruction;
    class UnaryInstruction;

    class BIBBLIR_EXPORT IRBuilder {
    public:
        BasicBlock* getInsertPoint() const;
        void setInsertPoint(BasicBlock* newInsertPoint);

        void insertAfter(Value* value);

        ConstantBoolean* createConstantBoolean(bool value);
        ConstantInt* createConstantInt(intmax_t value, Type* type);

        BinaryInstruction* createAdd(Value* left, Value* right);
        BinaryInstruction* createSub(Value* left, Value* right);
        BinaryInstruction* createMul(Value* left, Value* right);
        BinaryInstruction* createSDiv(Value* left, Value* right);
        BinaryInstruction* createUDiv(Value* left, Value* right);
        BinaryInstruction* createSMod(Value* left, Value* right);
        BinaryInstruction* createUMod(Value* left, Value* right);

        BinaryInstruction* createAnd(Value* left, Value* right);
        BinaryInstruction* createOr(Value* left, Value* right);
        BinaryInstruction* createXor(Value* left, Value* right);
        BinaryInstruction* createSHL(Value* left, Value* right);
        BinaryInstruction* createSHR(Value* left, Value* right);
        BinaryInstruction* createSAR(Value* left, Value* right);

        BinaryInstruction* createCmpEQ(Value* left, Value* right);
        BinaryInstruction* createCmpNE(Value* left, Value* right);
        BinaryInstruction* createCmpLT(Value* left, Value* right);
        BinaryInstruction* createCmpGT(Value* left, Value* right);
        BinaryInstruction* createCmpLE(Value* left, Value* right);
        BinaryInstruction* createCmpGE(Value* left, Value* right);


        UnaryInstruction* createNeg(Value* value);
        UnaryInstruction* createAbs(Value* value);
        UnaryInstruction* createNot(Value* value);


        PhiInstruction* createPhi(Type* type);

        ReturnInstruction* createReturn(Value* returnValue);

    private:
        BasicBlock* mInsertPoint = nullptr;
        Value* mInsertAfter = nullptr;
    };
}

#endif //BIBBLIR_IR_BUILDER_H
