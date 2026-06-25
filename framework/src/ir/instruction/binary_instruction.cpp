// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/binary_instruction.h"

#include "BibblIR/ir/basicblock.h"

#include "BibblIR/visitor/visitor.h"

#include "BibblIR/module.h"

#include <cassert>
#include <format>

namespace bibblir {
    BinaryInstruction::Operator BinaryInstruction::getOperator() const {
        return mOperator;
    }

    std::vector<std::reference_wrapper<Value*>> BinaryInstruction::getOperands() {
        return {mLeft, mRight};
    }

    std::string BinaryInstruction::identifier() const {
        return getName(mValueId);
    }

    void BinaryInstruction::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    BinaryInstruction::BinaryInstruction(BasicBlock* parent, Value* left, Operator op, Value* right)
        : Instruction(parent->getModule(), parent)
        , mLeft(left)
        , mOperator(op)
        , mRight(right)
        , mValueId(mModule.getNextValueId()) {
        assert(mLeft->getType() == mRight->getType());

        mLeft->forceRegister();
        mRight->forceRegister();

        switch (mOperator) {
            case ADD:
            case SUB:
            case MUL:
            case SDIV:
            case UDIV:
            case SMOD:
            case UMOD:
            case AND:
            case OR:
            case XOR:
            case SHL:
            case SHR:
            case SAR:
                mType = mLeft->getType();
                break;
            case EQ:
            case NE:
            case LT:
            case GT:
            case LE:
            case GE:
                mType = Type::GetBooleanType();
                break;
        }
    }
}
