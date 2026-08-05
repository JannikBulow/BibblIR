// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/branch_instruction.h"
#include "BibblIR/ir/instruction/phi_instruction.h"

#include "BibblIR/ir/builder.h"

#include "BibblIR/optimizer/ces/critical_edge_split.h"

namespace bibblir {
    void CriticalEdgeSplitter::run(Function* function) {
        std::vector<std::pair<BasicBlock*, BasicBlock*>> criticalEdges;

        for (auto& bb : function->basicBlocks()) {
            if (!bb->exists()) continue;
            if (bb->successors().size() <= 1) continue;

            for (BasicBlock* succ : bb->successors()) {
                if (succ->predecessors().size() > 1) {
                    criticalEdges.emplace_back(bb.get(), succ);
                }
            }
        }

        for (auto& edge : criticalEdges) {
            handleCriticalEdge(edge.first, edge.second);
        }
    }

    void CriticalEdgeSplitter::handleCriticalEdge(BasicBlock* from, BasicBlock* to) {
        Function* function = from->getParent();
        IRBuilder builder;
        BasicBlock* split = function->createBasicBlock(std::format("{}_to_{}_split", from->getName(), to->getName()));

        builder.setInsertPoint(split);
        builder.createBr(to);

        replaceSuccessor(from, to, split);
        replacePredecessor(to, from, split);

        for (auto& value : from->mValueList) {
            if (auto* branch = dynamic_cast<BranchInstruction*>(value.get())) {
                branch->replaceDestinations(to, split);
            }
        }

        for (PhiInstruction* phi : to->mPhis) {
            for (auto& incoming : phi->incoming()) {
                if (incoming.second == from) incoming.second = split;
            }
        }
    }

    void CriticalEdgeSplitter::replacePredecessor(BasicBlock* block, BasicBlock* oldPred, BasicBlock* newPred) {
        for (auto& pred : block->predecessors()) {
            if (pred == oldPred) pred = newPred;
        }
    }

    void CriticalEdgeSplitter::replaceSuccessor(BasicBlock* block, BasicBlock* oldSucc, BasicBlock* newSucc) {
        for (auto& succ : block->successors()) {
            if (succ == oldSucc) succ = newSucc;
        }
    }
}

