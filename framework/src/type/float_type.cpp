// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/type/float_type.h"

#include <cassert>
#include <format>

namespace bibblir {
    FloatType::FloatType(int bytes)
        : Type(bytes, std::format("float{}", bytes)) {}

    Type::ID FloatType::getID() const {
        switch (mSizeInBytes) {
            case 4: return ID::Float;
            case 8: return ID::Double;
            default: assert(false);
        }
    }

    bool FloatType::isFloatType() const {
        return true;
    }
}
