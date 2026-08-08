// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_PASS_CFGC_CFG_CANONICALIZE_H
#define BIBBLIR_PASS_CFGC_CFG_CANONICALIZE_H

#include "BibblIR/pass/pass.h"

#include <stack>

namespace bibblir {
    class BasicBlock;
    class Function;

    class BIBBLIR_EXPORT CFGCanonicalizationPass : public Pass {
    public:
        PassID getId() const override;

        std::vector<IRProperty> getProvidedProperties() const override;
        std::vector<IRProperty> getRequiredProperties() const override;

        void run(Module& module) override;

    private:
        static void postorder(BasicBlock* head, std::vector<BasicBlock*>& visited, std::stack<BasicBlock*>& stack);

        void run(Function* function);
    };
}

#endif //BIBBLIR_PASS_CFGC_CFG_CANONICALIZE_H
