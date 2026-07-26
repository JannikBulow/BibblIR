// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/type/class_type.h"

namespace bibblir {
    ClassType::ClassType()
        : Type(8, "class reference") {}

    Type::ID ClassType::getID() const {
        return ID::Reference;
    }

    bool ClassType::isClassType() const {
        return true;
    }
}
