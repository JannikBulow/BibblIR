// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/getelement_instruction.h"

#include "BibblIR/ir/basicblock.h"

#include "BibblIR/type/array_type.h"

#include "BibblIR/visitor/visitor.h"

#include "BibblIR/module.h"

#include <cassert>

namespace bibblir {
    std::vector<std::reference_wrapper<Value*>> GetElementInstruction::getOperands() {
        return {mArray, mIndex};
    }

    std::string GetElementInstruction::identifier() const {
        return getName(mValueId);
    }

    void GetElementInstruction::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    GetElementInstruction::GetElementInstruction(BasicBlock* parent, Value* array, Value* index)
        : Instruction(parent->getModule(), parent)
        , mArray(array)
        , mIndex(index)
        , mValueId(mModule.getNextValueId()) {
        assert(mArray->getType()->isArrayType());
        mType = static_cast<ArrayType*>(mArray->getType())->getElementType();
        mRequiresVReg = false;

        mIndex->forceRegister();
    }
}
