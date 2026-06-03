// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/module.h"

namespace bibblir {
    Module::Module(std::string name)
        : mName(std::move(name)) {}

    void Module::accept(Visitor& visitor) {
    }
}
