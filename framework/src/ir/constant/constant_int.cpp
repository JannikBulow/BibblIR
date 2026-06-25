// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/constant/constant_int.h"

#include "BibblIR/ir/basicblock.h"

#include "BibblIR/visitor/visitor.h"

#include <format>

namespace bibblir {
    bool ConstantInt::isConstant() const {
        return true;
    }

    std::string ConstantInt::identifier() const {
        if (mForceRegister) {
            return std::format("%{}", getName(-1)); // valueId does not matter cuz we know we have a vreg
        } else {
            return std::format("{} {}", mType->getName(), mValue);
        }
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
