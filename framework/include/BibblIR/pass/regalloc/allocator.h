// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_PASS_REGALLOC_ALLOCATOR_H
#define BIBBLIR_PASS_REGALLOC_ALLOCATOR_H

#include "BibblIR/ir/function.h"

#include "BibblIR/pass/regalloc/vreg.h"

#include "BibblIR/pass/pass.h"

namespace bibblir {
    class BIBBLIR_EXPORT RegAlloc {
    public:
        void assignVRegs(Function* function);

    private:
        void doRegAlloc(Function* function);

        void setLiveIntervals(Function* function);
    };

    class BIBBLIR_EXPORT RegAllocPass : public Pass {
    public:
        PassID getId() const override;

        std::vector<IRProperty> getProvidedProperties() const override;
        std::vector<IRProperty> getRequiredProperties() const override;

        void run(Module& module) override;
    };
}

#endif //BIBBLIR_PASS_REGALLOC_ALLOCATOR_H
