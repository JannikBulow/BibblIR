// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/type/boolean_type.h"

namespace bibblir {
    BooleanType::BooleanType()
        : Type(1, "boolean") {}

    bool BooleanType::isBooleanType() const {
        return true;
    }
}
