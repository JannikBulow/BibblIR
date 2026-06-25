// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_MODULE_H
#define BIBBLIR_MODULE_H

#include "BibblIR/ir/global.h"

#include "BibblIR/api.h"

#include <string>

namespace bibblir {
    class Visitor;

    class BIBBLIR_EXPORT Module {
    public:
        explicit Module(std::string name);

        const std::string& getName() const;
        int getNextValueId();

        const std::vector<GlobalPtr>& getGlobals() const;
        void insertGlobal(GlobalPtr global);
        void insertGlobalAtFront(GlobalPtr global);

        void accept(Visitor& visitor);

    private:
        std::string mName;

        int mNextValueId = 0;

        std::vector<GlobalPtr> mGlobals;
    };
}

#endif // BIBBLIR_MODULE_H
