// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_PASS_CES_CRITICAL_EDGE_SPLIT_H
#define BIBBLIR_PASS_CES_CRITICAL_EDGE_SPLIT_H

#include "BibblIR/ir/function.h"

#include "BibblIR/pass/pass.h"

namespace bibblir {
    class CriticalEdgeSplitter {
    public:
        void run(Function* function);

    private:
        void handleCriticalEdge(BasicBlock* from, BasicBlock* to);

        void replacePredecessor(BasicBlock* block, BasicBlock* oldPred, BasicBlock* newPred);
        void replaceSuccessor(BasicBlock* block, BasicBlock* oldSucc, BasicBlock* newSucc);
    };

    class BIBBLIR_EXPORT CESPass : public Pass {
    public:
        PassID getId() const override;

        std::vector<IRProperty> getProvidedProperties() const override;
        std::vector<IRProperty> getRequiredProperties() const override;

        std::vector<PassID> runBefore() const override;

        void run(Module& module) override;
    };
}

#endif //BIBBLIR_PASS_CES_CRITICAL_EDGE_SPLIT_H
