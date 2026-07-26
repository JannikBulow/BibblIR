// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/type/integer_type.h"

#include <cassert>
#include <format>

namespace bibblir {
    IntegerType::IntegerType(int bytes)
        : Type(bytes, std::format("int{}", bytes)) {}

    Type::ID IntegerType::getID() const {
        switch (mSizeInBytes) {
            case 1: return ID::Byte;
            case 2: return ID::Short;
            case 4: return ID::Int;
            case 8: return ID::Long;
            default: assert(false);
        }
    }

    bool IntegerType::isIntegerType() const {
        return true;
    }
}
