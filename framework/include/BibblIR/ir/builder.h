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
    class BranchInstruction;
    class CallInstruction;
    class FieldInstruction;
    class IntCastInstruction;
    class LoadInstruction;
    class NewInstruction;
    class PhiInstruction;
    class ReturnInstruction;
    class StoreInstruction;
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


        BranchInstruction* createBr(BasicBlock* destination);
        BranchInstruction* createCondBr(Value* condition, BasicBlock* trueBranch, BasicBlock* falseBranch);


        NewInstruction* createNew(Value* clas);


        LoadInstruction* createLoad(Value* variable);
        StoreInstruction* createStore(Value* variable, Value* value);
        FieldInstruction* createGetField(Value* object, Value* field);


        PhiInstruction* createPhi(Type* type);

        IntCastInstruction* createIntCast(Value* value, Type* destType);


        CallInstruction* createCall(Value* callee, std::vector<Value*> parameters);


        ReturnInstruction* createReturn(Value* returnValue);

    private:
        BasicBlock* mInsertPoint = nullptr;
        Value* mInsertAfter = nullptr;
    };
}

#endif //BIBBLIR_IR_BUILDER_H
