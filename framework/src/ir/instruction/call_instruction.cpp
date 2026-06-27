// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/call_instruction.h"

#include "BibblIR/ir/basicblock.h"
#include "BibblIR/ir/function.h"

#include "BibblIR/type/function_type.h"

#include "BibblIR/visitor/visitor.h"

#include "BibblIR/module.h"

#include <cassert>

namespace bibblir {
    std::vector<std::reference_wrapper<Value*>> CallInstruction::getOperands() {
        std::vector<std::reference_wrapper<Value*>> operands;
        operands.reserve(mParameters.size());

        for (auto& parameter : mParameters) {
            operands.emplace_back(parameter);
        }

        if (!dynamic_cast<Function*>(mCallee->getType())) {
            operands.emplace_back(mCallee);
        }

        return operands;
    }

    std::string CallInstruction::identifier() const {
        return getName(mValueId);
    }

    void CallInstruction::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    CallInstruction::CallInstruction(BasicBlock* parent, Value* callee, std::vector<Value*> parameters)
        : Instruction(parent->getModule(), parent)
        , mCallee(callee)
        , mParameters(std::move(parameters))
        , mValueId(mModule.getNextValueId()) {
        FunctionType* functionType = dynamic_cast<FunctionType*>(mCallee->getType());
        assert(functionType);

        mType = functionType->getReturnType();

        assert(mParameters.size() == functionType->getArgumentTypes().size());

        int index = 0;
        for (Value* parameter : mParameters) {
            assert(parameter->getType() == functionType->getArgumentTypes()[index]);
            index++;
        }

        mVRegRangeSize = mParameters.size();
    }
}
