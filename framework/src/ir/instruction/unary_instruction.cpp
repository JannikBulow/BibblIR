// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/unary_instruction.h"

#include "BibblIR/ir/basicblock.h"

#include "BibblIR/visitor/visitor.h"

#include "BibblIR/module.h"

namespace bibblir {
    UnaryInstruction::Operator UnaryInstruction::getOperator() const {
        return mOperator;
    }

    std::vector<std::reference_wrapper<Value*>> UnaryInstruction::getOperands() {
        return {mOperand};
    }

    std::string UnaryInstruction::identifier() const {
        return getName(mValueId);
    }

    void UnaryInstruction::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    UnaryInstruction::UnaryInstruction(BasicBlock* parent, Value* operand, Operator op)
        : Instruction(parent->getModule(), parent)
        , mOperand(operand)
        , mOperator(op)
        , mValueId(mModule.getNextValueId()) {
        mOperand->forceRegister();

        switch (mOperator) {
            case NEG:
            case ABS:
            case NOT:
                mType = mOperand->getType();
                break;
        }
    }
}
