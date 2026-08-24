// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/class.h"
#include "BibblIR/ir/field.h"

#include "BibblIR/visitor/visitor.h"

#include <format>

namespace bibblir {
    std::string Field::identifier() const {
        return std::format("field %{}::{}", mParent->getName(), mName);
    }

    void Field::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    Field::Field(AbstractClass* parent, Type* type, std::string name)
        : Value(parent->getModule())
        , mParent(parent)
        , mName(std::move(name)) {
        mType = type;
        mRequiresVReg = false;
    }
}