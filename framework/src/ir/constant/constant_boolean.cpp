// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/constant/constant_boolean.h"

#include "BibblIR/ir/basicblock.h"

#include "BibblIR/visitor/visitor.h"

#include <format>

namespace bibblir {
    bool ConstantBoolean::getValue() const {
        return mValue;
    }

    bool ConstantBoolean::isConstant() const {
        return true;
    }

    std::string ConstantBoolean::identifier() const {
        return std::format("{} {}", mType->getName(), mValue);
    }

    void ConstantBoolean::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    ConstantBoolean::ConstantBoolean(BasicBlock* parent, bool value)
        : Value(parent->getModule())
        , mValue(value) {
        mType = Type::GetBooleanType();
        mRequiresVReg = false;
    }
}
