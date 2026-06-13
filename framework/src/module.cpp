// Copyright 2026 Jannik Laugmand Bülow

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

    void Module::insertGlobal(GlobalPtr global) {
        mGlobals.push_back(std::move(global));
    }

    void Module::insertGlobalAtFront(GlobalPtr global) {
        mGlobals.insert(mGlobals.begin(), std::move(global));
    }

    const std::vector<ValuePtr>& Module::getConstants() const {
        return mConstants;
    }

    void Module::insertConstant(ValuePtr constant) {
        mConstants.push_back(std::move(constant));
    }

    void Module::accept(Visitor& visitor) {
        visitor.visit(*this);
    }
}
