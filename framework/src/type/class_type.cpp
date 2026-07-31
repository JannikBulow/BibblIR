// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/type/class_type.h"

#include <format>

namespace bibblir {
    ClassType::ClassType(std::string moduleName, std::string name)
        : Type(8, std::format("{}::{}", moduleName, name))
        , mModuleName(std::move(moduleName))
        , mClassName(std::move(name)) {}

    std::string_view ClassType::getModuleName() const {
        return mModuleName;
    }

    std::string_view ClassType::getClassName() const {
        return mClassName;
    }

    Type::ID ClassType::getID() const {
        return ID::Reference;
    }

    bool ClassType::isClassType() const {
        return true;
    }
}
