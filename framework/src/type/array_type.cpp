// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/type/array_type.h"

#include <format>

namespace bibblir {
    ArrayType::ArrayType(Type* elementType)
        : Type(8, std::format("{}[]", elementType->getName()))
        , mElementType(elementType) {}

    Type* ArrayType::getElementType() const {
        return mElementType;
    }

    Type::ID ArrayType::getID() const {
        return ID::Reference;
    }

    bool ArrayType::isArrayType() const {
        return true;
    }
}
