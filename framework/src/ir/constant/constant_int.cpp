// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/constant/constant_int.h"

#include "BibblIR/ir/basicblock.h"

#include "BibblIR/visitor/visitor.h"

#include <format>

namespace bibblir {
    intmax_t ConstantInt::getValue() const {
        return mValue;
    }

    bool ConstantInt::isConstant() const {
        return true;
    }

    std::string ConstantInt::identifier() const {
        return std::format("{} {}", mType->getName(), mValue);
    }

    void ConstantInt::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    ConstantInt::ConstantInt(BasicBlock* parent, intmax_t value, Type* type)
        : Value(parent->getModule())
        , mValue(value) {
        mType = type;
        mRequiresVReg = false;
    }
}
