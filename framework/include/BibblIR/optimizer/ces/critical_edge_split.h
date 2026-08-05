// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_OPTIMIZER_CES_CRITICAL_EDGE_SPLIT_H
#define BIBBLIR_OPTIMIZER_CES_CRITICAL_EDGE_SPLIT_H

#include "BibblIR/ir/function.h"

namespace bibblir {
    class CriticalEdgeSplitter {
    public:
        void run(Function* function);

    private:
        void handleCriticalEdge(BasicBlock* from, BasicBlock* to);

        void replacePredecessor(BasicBlock* block, BasicBlock* oldPred, BasicBlock* newPred);
        void replaceSuccessor(BasicBlock* block, BasicBlock* oldSucc, BasicBlock* newSucc);
    };
}

#endif //BIBBLIR_OPTIMIZER_CES_CRITICAL_EDGE_SPLIT_H
