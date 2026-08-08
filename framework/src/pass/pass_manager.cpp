// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/pass/pass_manager.h"

#include "BibblIR/module.h"

#include <cassert>
#include <queue>
#include <unordered_map>

namespace bibblir {
    void PassPipeline::run(Module& module) {
        for (const auto& pass : mPasses) {
            pass->run(module);
        }
    }

    PassPipeline::PassPipeline(std::vector<PassPtr> passes)
        : mPasses(std::move(passes)) {}

    PassManager::PassManager(PassRegistry& passRegistry)
        : mPassRegistry(passRegistry) {}

    void PassManager::addPass(PassPtr pass) {
        mRequestedPasses.push_back(std::move(pass));
    }

    PassPipeline PassManager::buildPipeline() {
        std::vector<PassPtr> passes = std::move(mRequestedPasses);
        std::unordered_map<PassID, PassNodePtr> nodes;
        nodes.reserve(passes.size());

        for (auto& pass : passes) {
            ensurePass(nodes, passes, pass->getId());
        }

        for (auto& [id, node] : nodes) {
            Pass& pass = *passes[node->passIndex];

            for (PassID before : pass.runBefore()) {
                auto it = nodes.find(before);
                if (it == nodes.end()) continue;

                addDependency(nodes, id, before);
            }

            for (PassID after : pass.runAfter()) {
                auto it = nodes.find(after);
                if (it == nodes.end()) continue;

                addDependency(nodes, after, id);
            }
        }

        std::vector<PassNode*> sorted;
        sorted.reserve(passes.size());

        for (auto& [id, node] : nodes) {
            if (node->dependencyCount == 0) sorted.push_back(node.get());
        }

        for (size_t i = 0; i < sorted.size(); i++) {
            for (PassNode* dependent : sorted[i]->dependents) {
                if (--dependent->dependencyCount == 0) sorted.push_back(dependent);
            }
        }

        assert(sorted.size() == nodes.size());

        std::vector<PassPtr> ordered;
        ordered.reserve(passes.size());

        for (PassNode* node : sorted) {
            ordered.push_back(std::move(passes[node->passIndex]));
        }

        return PassPipeline(std::move(ordered));
    }

    void PassManager::addDependency(std::unordered_map<PassID, PassNodePtr>& nodes, PassID dependency, PassID dependent) {
        PassNode& dependencyNode = *nodes.at(dependency);
        PassNode& dependentNode = *nodes.at(dependent);

        dependencyNode.dependents.push_back(&dependentNode);
        dependentNode.dependencyCount++;
    }

    PassManager::PassNode& PassManager::ensurePass(std::unordered_map<PassID, PassNodePtr>& nodes, std::vector<PassPtr>& passes, PassID id) {
        auto it = nodes.find(id);
        if (it != nodes.end()) return *it->second;

        PassPtr pass = mPassRegistry.create(id);
        assert(pass);

        size_t index = passes.size();
        passes.push_back(std::move(pass));

        auto [nodeIt, inserted] = nodes.emplace(id, std::make_unique<PassNode>(index));

        assert(inserted);

        PassNode& node = *nodeIt->second;
        Pass& currentPass = *passes[index];

        for (IRProperty property : currentPass.getRequiredProperties()) {
            PassID provider = mPassRegistry.getProvider(property);

            PassNode& dependency = ensurePass(nodes, passes, provider);

            dependency.dependents.push_back(&node);
            node.dependencyCount++;
        }

        return node;
    }
}
