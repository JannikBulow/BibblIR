// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_PASS_REGISTRY_H
#define BIBBLIR_PASS_REGISTRY_H

#include "BibblIR/pass/pass.h"

#include <cassert>
#include <functional>
#include <span>

namespace bibblir {
    template<class T>
    using PassFactory = std::function<std::unique_ptr<T>()>;

    using PassFactory2 = std::function<PassPtr()>;

    class BIBBLIR_EXPORT PassRegistry {
    public:
        static PassRegistry Default();

        template<class T>
        void registerPass(PassFactory<T> factory = [] { return std::make_unique<T>(); }) {
            PassID id = GetPassID<T>();

            PassPtr pass = factory();
            for (IRProperty property : pass->getProvidedProperties()) {
                mProviders[property].push_back(id);
            }

            mFactories[id] = std::move(factory);
        }

        PassID getProvider(IRProperty property) const;
        std::span<const PassID> getProviders(IRProperty property) const;
        PassPtr create(PassID id);

    private:
        std::unordered_map<IRProperty, std::vector<PassID>> mProviders;
        std::unordered_map<PassID, PassFactory2> mFactories;
    };
}

#endif //BIBBLIR_PASS_REGISTRY_H
