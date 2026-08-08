// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_PASS_OPTIMIZER_PEEPHOLE_H
#define BIBBLIR_PASS_OPTIMIZER_PEEPHOLE_H

#include "BibblIR/pass/pass.h"

#include <BibbleASM/codegen/assembler.h>

#include <unordered_set>

namespace bibblir {
    class BIBBLIR_EXPORT PeepholePass : public Pass {
    public:
        PassID getId() const override;

        std::vector<IRProperty> getProvidedProperties() const override;
        std::vector<IRProperty> getRequiredProperties() const override;

        std::vector<PassID> runAfter() const override;

        void run(Module& module) override;

    private:
        enum class UnreachableLabelLevel {
            Unreachable,
            OptimizedReferences,
            References
        };

        std::unordered_set<std::string> mReferenceOptimizedLabels;

        void run(bibbleasm::Assembler& assembler);

        UnreachableLabelLevel checkUnreachableLabel(bibbleasm::Assembler& assembler, bibbleasm::AssemblerNode& node);
        bool checkJmpLabel(bibbleasm::AssemblerNode& first, bibbleasm::AssemblerNode& second);

        std::vector<bibbleasm::AssemblerNode>::iterator eraseNode(std::vector<bibbleasm::AssemblerNode>& nodes, std::vector<bibbleasm::AssemblerNode>::iterator it);
    };
}

#endif //BIBBLIR_PASS_OPTIMIZER_PEEPHOLE_H
