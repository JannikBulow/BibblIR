// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/constant/constant_boolean.h"
#include "BibblIR/ir/constant/constant_int.h"

#include "BibblIR/ir/instruction/binary_instruction.h"
#include "BibblIR/ir/instruction/return_instruction.h"

#include "BibblIR/ir/basicblock.h"
#include "BibblIR/ir/builder.h"

namespace bibblir {
    BasicBlock* IRBuilder::getInsertPoint() const {
        return mInsertPoint;
    }

    void IRBuilder::setInsertPoint(BasicBlock* newInsertPoint) {
        mInsertPoint = newInsertPoint;
    }

    void IRBuilder::insertAfter(Value* value) {
        mInsertAfter = value;
    }

    ConstantBoolean* IRBuilder::createConstantBoolean(bool value) {
        ConstantBoolean* constant = new ConstantBoolean(mInsertPoint, value);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(constant));

        return constant;
    }

    ConstantInt* IRBuilder::createConstantInt(intmax_t value, Type* type) {
        ConstantInt* constant = new ConstantInt(mInsertPoint, value, type);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(constant));

        return constant;
    }

    BinaryInstruction* IRBuilder::createBinaryInstruction(Value* left, BinaryInstruction::Operator op, Value* right) {
        BinaryInstruction* inst = new BinaryInstruction(mInsertPoint, left, op, right);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(inst));

        return inst;
    }

    ReturnInstruction* IRBuilder::createReturn(Value* returnValue) {
        ReturnInstruction* inst = new ReturnInstruction(mInsertPoint, returnValue);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(inst));

        return inst;
    }
}
