// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/external_function.h"

#include "BibblIR/visitor/visitor.h"

#include "BibblIR/module.h"

namespace bibblir {
    ExternalFunction* ExternalFunction::Create(Module& module, FunctionType* type, std::string moduleName, std::string name) {
        ExternalFunction* func = new ExternalFunction(module, type, std::move(moduleName), std::move(name));
        module.insertGlobal(GlobalPtr(func));
        return func;
    }

    FunctionType* ExternalFunction::getFunctionType() const {
        return static_cast<FunctionType*>(mType);
    }

    std::string_view ExternalFunction::getModuleName() const {
        return mModuleName;
    }

    std::string_view ExternalFunction::getName() const {
        return mName;
    }

    std::string ExternalFunction::identifier() const {
        return mModuleName + "::" + mName;
    }

    void ExternalFunction::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    ExternalFunction::ExternalFunction(Module& module, FunctionType* type, std::string moduleName, std::string name)
        : AbstractFunction(module)
        , mModuleName(std::move(moduleName))
        , mName(std::move(name)) {
        mType = type;
    }
}
