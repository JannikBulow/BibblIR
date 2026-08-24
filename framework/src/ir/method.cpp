// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/class.h"
#include "BibblIR/ir/method.h"

#include "BibblIR/visitor/visitor.h"

#include <format>

namespace bibblir {
    FunctionType* Method::getFunctionType() const {
        return static_cast<FunctionType*>(mType);
    }

    bool Method::isAbstract() const {
        return mImpl == nullptr;
    }

    std::string Method::identifier() const {
        return std::format("method %{}::{}", mParent->getName(), mName);
    }

    void Method::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    Method::Method(AbstractClass* parent, FunctionType* type, std::string name, Value* impl)
        : Value(parent->getModule())
        , mParent(parent)
        , mName(std::move(name))
        , mImpl(impl) {
        mType = type;
        mRequiresVReg = false;
    }
}
