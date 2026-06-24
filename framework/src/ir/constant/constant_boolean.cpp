// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/constant/constant_boolean.h"

#include "BibblIR/visitor/visitor.h"

#include "BibblIR/module.h"

#include <format>

namespace bibblir {
    ConstantBoolean* ConstantBoolean::Get(Module& module, bool value) {
        ConstantBoolean* constant = new ConstantBoolean(module, value);
        module.insertConstant(ValuePtr(constant));
        return constant;
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

    ConstantBoolean::ConstantBoolean(Module& module, bool value)
        : Value(module)
        , mValue(value) {
        mType = Type::GetBooleanType();
    }
}
