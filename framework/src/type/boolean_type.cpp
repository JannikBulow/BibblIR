// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/type/boolean_type.h"

namespace bibblir {
    BooleanType::BooleanType()
        : Type(1, "boolean") {}

    Type::ID BooleanType::getID() const {
        return ID::Byte;
    }

    bool BooleanType::isBooleanType() const {
        return true;
    }
}
