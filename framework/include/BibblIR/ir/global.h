// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_GLOBAL_H
#define BIBBLIR_IR_GLOBAL_H

#include "BibblIR/ir/value.h"

namespace bibblir {
    class Global : public Value {
    public:
        Global(Module& module) : Value(module) {}
        ~Global() override = default;
    };
}

#endif //BIBBLIR_IR_GLOBAL_H
