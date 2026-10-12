// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/type/string_type.h"

namespace bibblir {
    StringType::StringType()
        : Type(8, "string") {}

    Type::ID StringType::getID() const {
        return Type::ID::Reference;
    }

    bool StringType::isStringType() const {
        return true;
    }
}
