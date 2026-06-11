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

        std::string_view getName() const;

        const std::vector<GlobalPtr>& getGlobals() const;
        void insertGlobal(GlobalPtr global);
        void insertGlobalAtFront(GlobalPtr global);

        const std::vector<ValuePtr>& getConstants() const;
        void insertConstant(ValuePtr constant);

        void accept(Visitor& visitor);

    private:
        std::string mName;

        std::vector<GlobalPtr> mGlobals;
        std::vector<ValuePtr> mConstants;
    };
}

#endif // BIBBLIR_MODULE_H
