// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/type/integer_type.h"

#include <format>

namespace bibblir {
    IntegerType::IntegerType(int bytes)
        : Type(bytes, std::format("int{}", bytes)) {}

    bool IntegerType::isIntegerType() const {
        return true;
    }
}
