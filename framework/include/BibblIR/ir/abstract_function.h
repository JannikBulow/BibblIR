// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_ABSTRACT_FUNCTION_H
#define BIBBLIR_IR_ABSTRACT_FUNCTION_H

#include "BibblIR/ir/global.h"

namespace bibblir {
    class BIBBLIR_EXPORT AbstractFunction : public Global {
    public:
        using Global::Global;

        virtual std::string_view getModuleName() const = 0;
        virtual std::string_view getName() const = 0;
    };
}

#endif //BIBBLIR_IR_ABSTRACT_FUNCTION_H
