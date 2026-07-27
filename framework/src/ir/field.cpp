// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/class.h"
#include "BibblIR/ir/field.h"

#include "BibblIR/visitor/visitor.h"

namespace bibblir {
    std::string Field::identifier() const {
        return "field %" + mParent->mName + "::" + mName;
    }

    void Field::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    Field::Field(Class* parent, Type* type, std::string name)
        : Value(parent->getModule())
        , mName(std::move(name)) {
        mType = type;
        mRequiresVReg = false;
    }
}