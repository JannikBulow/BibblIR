// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/constant/constant_int.h"

#include "BibblIR/visitor/visitor.h"

#include "BibblIR/module.h"

#include <format>

namespace bibblir {
    ConstantInt* ConstantInt::Get(Module& module, intmax_t value, Type* type) {
        ConstantInt* constant = new ConstantInt(module, value, type);
        module.insertConstant(ValuePtr(constant));
        return constant;
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

    ConstantInt::ConstantInt(Module& module, intmax_t value, Type* type)
        : Value(module)
        , mValue(value) {
        mType = type;
    }
}
