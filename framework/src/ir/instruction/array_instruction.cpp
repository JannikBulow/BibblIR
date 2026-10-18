// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/array_instruction.h"

#include "BibblIR/ir/basicblock.h"

#include "BibblIR/visitor/visitor.h"

#include "BibblIR/module.h"

#include <cassert>

namespace bibblir {
    ArrayInstruction::Operator ArrayInstruction::getOperator() const {
        return mOperator;
    }

    Value* ArrayInstruction::getArrayOperand() const {
        return mArrayOperand;
    }

    Value* ArrayInstruction::getOtherOperand() const {
        return mOtherOperand;
    }

    std::vector<std::reference_wrapper<Value*>> ArrayInstruction::getOperands() {
        std::vector<std::reference_wrapper<Value*>> operands;
        operands.emplace_back(mArrayOperand);
        if (mOtherOperand) operands.emplace_back(mOtherOperand);
        return operands;
    }

    std::string ArrayInstruction::identifier() const {
        return getName(mValueId);
    }

    void ArrayInstruction::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    ArrayInstruction::ArrayInstruction(BasicBlock* parent, Operator op, Value* arrayOperand, Value* otherOperand)
        : Instruction(parent->getModule(), parent)
        , mOperator(op)
        , mArrayOperand(arrayOperand)
        , mOtherOperand(otherOperand)
        , mValueId(mModule.getNextValueId()) {
        assert(mArrayOperand->getType()->isArrayType());

        switch (mOperator) {
            case Length:
                assert(mOtherOperand == nullptr);
                mType = Type::GetIntegerType(8);
                break;
        }
    }
}
