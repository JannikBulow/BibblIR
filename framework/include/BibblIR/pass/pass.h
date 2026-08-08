// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_PASS_PASS_H
#define BIBBLIR_PASS_PASS_H

#include "BibblIR/pass/property.h"

#include "BibblIR/api.h"

#include <memory>
#include <vector>

namespace bibblir {
    class Module;

    using PassID = const void*;

    template<class T>
    PassID GetPassID() {
        static const char id = 67;
        return &id;
    }

    class BIBBLIR_EXPORT Pass {
    public:
        virtual ~Pass() = default;

        virtual PassID getId() const = 0;

        virtual std::vector<IRProperty> getProvidedProperties() const = 0;
        virtual std::vector<IRProperty> getRequiredProperties() const = 0;

        virtual std::vector<PassID> runBefore() const { return {}; } // insert me before these
        virtual std::vector<PassID> runAfter() const { return {}; } // insert me after these

        virtual void run(Module& module) = 0;
    };

    using PassPtr = std::unique_ptr<Pass>;
}

#endif //BIBBLIR_PASS_PASS_H
