// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_PASS_CODEGEN_CODEGEN_H
#define BIBBLIR_PASS_CODEGEN_CODEGEN_H

#include "BibblIR/pass/pass.h"

namespace bibblir {
    class BIBBLIR_EXPORT CodegenPass : public Pass {
    public:
        PassID getId() const override;

        std::vector<IRProperty> getProvidedProperties() const override;
        std::vector<IRProperty> getRequiredProperties() const override;

        void run(Module& module) override;
    };
}

#endif //BIBBLIR_PASS_CODEGEN_CODEGEN_H
