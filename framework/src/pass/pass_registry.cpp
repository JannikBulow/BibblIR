// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/pass/ces/critical_edge_split.h"

#include "BibblIR/pass/cfgc/cfg_canonicalize.h"

#include "BibblIR/pass/codegen/codegen.h"

#include "BibblIR/pass/regalloc/allocator.h"

#include "BibblIR/pass/pass_registry.h"

namespace bibblir {
    PassRegistry PassRegistry::Default() {
        PassRegistry passRegistry;

        // main providers. these are registered first so they're chosen first when a property is required
        passRegistry.registerPass<CESPass>();
        passRegistry.registerPass<CFGCanonicalizationPass>();
        passRegistry.registerPass<CodegenPass>();
        passRegistry.registerPass<RegAllocPass>();

        return passRegistry;
    }

    PassID PassRegistry::getProvider(IRProperty property) const {
        auto it = mProviders.find(property);
        assert(it != mProviders.end() && "no pass provides requested IR property");
        return it->second.front();
    }

    std::span<const PassID> PassRegistry::getProviders(IRProperty property) const {
        auto it = mProviders.find(property);
        assert(it != mProviders.end() && "no pass provides requested property");
        return it->second;
    }

    PassPtr PassRegistry::create(PassID id) {
        auto it = mFactories.find(id);
        if (it == mFactories.end()) return nullptr;

        return it->second();
    }
}
