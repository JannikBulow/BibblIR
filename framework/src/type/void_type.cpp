// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/type/void_type.h"

namespace bibblir {
    VoidType::VoidType()
        : Type(0, "void") {}

    bool VoidType::isVoidType() const {
        return true;
    }
}
