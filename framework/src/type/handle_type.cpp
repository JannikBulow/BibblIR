// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/type/handle_type.h"

namespace bibblir {
    HandleType::HandleType()
        : Type(8, "handle") {}

    Type::ID HandleType::getID() const {
        return ID::Handle;
    }

    bool HandleType::isHandleType() const {
        return true;
    }
}
