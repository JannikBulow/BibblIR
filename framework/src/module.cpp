// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/load_instruction.h"

#include "BibblIR/ir/function.h"

#include "BibblIR/visitor/visitor.h"

#include "BibblIR/module.h"

namespace bibblir {
    Module::Module(std::string name)
        : mName(std::move(name)) {}

    const std::string& Module::getName() const {
        return mName;
    }

    int Module::getNextValueId() {
        return ++mNextValueId;
    }

    const std::vector<GlobalPtr>& Module::getGlobals() const {
        return mGlobals;
    }

    const std::vector<Function*>& Module::getFunctions() const {
        return mFunctions;
    }

    void Module::insertGlobal(GlobalPtr global) {
        if (auto* function = dynamic_cast<Function*>(global.get())) mFunctions.push_back(function);
        mGlobals.push_back(std::move(global));
    }

    void Module::insertGlobalAtFront(GlobalPtr global) {
        if (auto* function = dynamic_cast<Function*>(global.get())) mFunctions.push_back(function);
        mGlobals.insert(mGlobals.begin(), std::move(global));
    }

    void Module::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    Value* GetVariableOperand(Value* value) {
        if (auto load = dynamic_cast<LoadInstruction*>(value)) {
            return load->getVariable();
        }
        return nullptr;
    }
}
