// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_MODULE_H
#define BIBBLIR_MODULE_H

#include "BibblIR/ir/global.h"

#include "BibblIR/api.h"

#include <BibbleASM/codegen/builder/module_builder.h>

#include <string>

namespace bibblir {
    class Function;
    class Visitor;

    class BIBBLIR_EXPORT Module {
    public:
        explicit Module(std::string name);

        const std::string& getName() const;
        int getNextValueId();

        const std::vector<GlobalPtr>& getGlobals() const;
        const std::vector<Function*>& getFunctions() const;
        void insertGlobal(GlobalPtr global);
        void insertGlobalAtFront(GlobalPtr global);

        void accept(Visitor& visitor);

        bibbleasm::ModuleBuilder& bytecodeModule();

    private:
        std::string mName;

        int mNextValueId = 0;

        std::vector<GlobalPtr> mGlobals;
        std::vector<Function*> mFunctions; // functions are needed so often that it's probably best to cache them

        bibbleasm::ModuleBuilder mBytecodeModule;
    };

    BIBBLIR_EXPORT Value* GetVariableOperand(Value* value);
}

#endif // BIBBLIR_MODULE_H
