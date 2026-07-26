// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/type/void_type.h"

namespace bibblir {
    VoidType::VoidType()
        : Type(0, "void") {}

    Type::ID VoidType::getID() const {
        return ID::Handle; // don't use a void type as a value type please
    }

    bool VoidType::isVoidType() const {
        return true;
    }
}
