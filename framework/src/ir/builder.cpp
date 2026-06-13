// Copyright 2026 Jannik Laugmand Bülow

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

    ReturnInstruction* IRBuilder::createReturn(Value* returnValue) {
        ReturnInstruction* inst = new ReturnInstruction(mInsertPoint, returnValue);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(inst));

        return inst;
    }
}
