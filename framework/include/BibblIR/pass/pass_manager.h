// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_PASS_PASS_MANAGER_H
#define BIBBLIR_PASS_PASS_MANAGER_H

#include "BibblIR/pass/pass.h"
#include "BibblIR/pass/pass_registry.h"

#include <unordered_map>

namespace bibblir {
    class BIBBLIR_EXPORT PassPipeline {
        friend class PassManager;
    public:
        void run(Module& module);

    private:
        std::vector<PassPtr> mPasses;

        explicit PassPipeline(std::vector<PassPtr> passes);
    };

    class BIBBLIR_EXPORT PassManager {
    public:
        explicit PassManager(PassRegistry& passRegistry);

        void addPass(PassPtr pass);

        // resets requested passes vector
        PassPipeline buildPipeline();

    private:
        struct PassNode {
            size_t passIndex;
            size_t dependencyCount = 0;
            std::vector<PassNode*> dependents;
        };

        using PassNodePtr = std::unique_ptr<PassNode>;

        PassRegistry& mPassRegistry;
        std::vector<PassPtr> mRequestedPasses;

        void addDependency(std::unordered_map<PassID, PassNodePtr>& nodes, PassID dependency, PassID dependent);
        PassNode& ensurePass(std::unordered_map<PassID, PassNodePtr>& nodes, std::vector<PassPtr>& passes, PassID id);
    };
}

#endif //BIBBLIR_PASS_PASS_MANAGER_H
