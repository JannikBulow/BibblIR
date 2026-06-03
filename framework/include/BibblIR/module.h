// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_MODULE_H
#define BIBBLIR_MODULE_H

#include "BibblIR/api.h"

#include <string>

namespace bibblir {
    class Visitor;

    class BIBBLIR_EXPORT Module {
    public:
        explicit Module(std::string name);

        std::string_view getName() const { return mName; }

        void accept(Visitor& visitor);

    private:
        std::string mName;
    };
}

#endif // BIBBLIR_MODULE_H
