// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_OPTIMIZER_REGALLOC_ALLOCATOR_H
#define BIBBLIR_OPTIMIZER_REGALLOC_ALLOCATOR_H

#include "BibblIR/ir/function.h"

#include "BibblIR/optimizer/regalloc/vreg.h"

#include <map>

namespace bibblir {
    // I could easily make this a visitor, but regalloc without codegen is unnecessary and codegen without regalloc will result in a segfault
    class BIBBLIR_EXPORT RegAlloc {
    public:
        void assignVRegs(Function* function);

    private:
        void doRegAlloc(Function* function);

        void setLiveIntervals(Function* function);
    };
}

#endif //BIBBLIR_OPTIMIZER_REGALLOC_ALLOCATOR_H
