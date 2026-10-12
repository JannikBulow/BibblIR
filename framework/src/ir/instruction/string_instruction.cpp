// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/string_instruction.h"

#include "BibblIR/ir/basicblock.h"

#include "BibblIR/visitor/visitor.h"

#include "BibblIR/module.h"

#include <cassert>

namespace bibblir {
    StringInstruction::Operator StringInstruction::getOperator() const {
        return mOperator;
    }

    Value* StringInstruction::getStringOperand() const {
        return mStringOperand;
    }

    Value* StringInstruction::getOtherOperand() const {
        return mOtherOperand;
    }

    std::vector<std::reference_wrapper<Value*>> StringInstruction::getOperands() {
        std::vector<std::reference_wrapper<Value*>> operands;
        operands.emplace_back(mStringOperand);
        if (mOtherOperand) operands.emplace_back(mOtherOperand);
        return operands;
    }

    std::string StringInstruction::identifier() const {
        return getName(mValueId);
    }

    void StringInstruction::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    StringInstruction::StringInstruction(BasicBlock* parent, Operator op, Value* stringOperand, Value* otherOperand)
        : Instruction(parent->getModule(), parent)
        , mOperator(op)
        , mStringOperand(stringOperand)
        , mOtherOperand(otherOperand)
        , mValueId(mModule.getNextValueId()) {
        assert(mStringOperand->getType()->isStringType());

        switch (mOperator) {
            case Length:
                assert(mOtherOperand == nullptr);
                mType = Type::GetIntegerType(8);
                break;
        }
    }
}
