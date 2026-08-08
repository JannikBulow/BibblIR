// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/function.h"

#include "BibblIR/pass/cfgc/cfg_canonicalize.h"

#include "BibblIR/module.h"

namespace bibblir {
    PassID CFGCanonicalizationPass::getId() const {
        return GetPassID<CFGCanonicalizationPass>();
    }

    std::vector<IRProperty> CFGCanonicalizationPass::getProvidedProperties() const {
        return {IRProperty::CanonicalCFG};
    }

    std::vector<IRProperty> CFGCanonicalizationPass::getRequiredProperties() const {
        return {};
    }

    void CFGCanonicalizationPass::run(Module& module) {
        for (Function* function : module.getFunctions()) {
            run(function);
        }
    }

    void CFGCanonicalizationPass::postorder(BasicBlock* head, std::vector<BasicBlock*>& visited, std::stack<BasicBlock*>& stack) {
        if (std::ranges::find(visited, head) != visited.end()) return;

        visited.push_back(head);
        for (BasicBlock* successor : head->successors()) {
            postorder(successor, visited, stack);
        }
        stack.push(head);
    }

    void CFGCanonicalizationPass::run(Function* function) {
        if (function->basicBlocks().empty()) return;

        std::vector<BasicBlock*> visited;
        std::stack<BasicBlock*> stack;
        postorder(function->basicBlocks().front().get(), visited, stack);

        std::vector<BasicBlock*> blocks;
        for (auto& bb : function->basicBlocks()) {
            blocks.push_back(bb.release());
        }

        function->basicBlocks().clear();

        while (!stack.empty()) {
            BasicBlock* bb = stack.top();
            stack.pop();

            function->basicBlocks().push_back(BasicBlockPtr(bb));
            std::erase(blocks, bb);
        }

        for (BasicBlock* bb : blocks) {
            bb->disable();
            function->basicBlocks().push_back(BasicBlockPtr(bb));
        }
    }
}
