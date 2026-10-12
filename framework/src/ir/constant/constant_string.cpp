// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/constant/constant_string.h"

#include "BibblIR/visitor/visitor.h"

namespace bibblir {
    std::string_view ConstantString::getValue() const {
        return mValue;
    }

    bool ConstantString::isConstant() const {
        return true;
    }

    std::string ConstantString::identifier() const {
        return std::format("{} \"{}\"", mType->getName(), mValue);
    }

    void ConstantString::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    ConstantString::ConstantString(Module& module, std::string value)
        : Value(module)
        , mValue(std::move(value)) {
        mType = Type::GetStringType();
        mRequiresVReg = false;
    }
}
