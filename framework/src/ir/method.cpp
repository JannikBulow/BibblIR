// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/class.h"
#include "BibblIR/ir/method.h"

#include "BibblIR/visitor/visitor.h"

namespace bibblir {
    FunctionType* Method::getFunctionType() const {
        return static_cast<FunctionType*>(mType);
    }

    bool Method::isAbstract() const {
        return mImpl == nullptr;
    }

    std::string Method::identifier() const {
        return "method %" + mParent->mName + "::" + mName;
    }

    void Method::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    Method::Method(Class* parent, FunctionType* type, std::string name, Value* impl)
        : Value(parent->getModule())
        , mParent(parent)
        , mName(std::move(name))
        , mImpl(impl) {
        mType = type;
        mRequiresVReg = false;
    }
}
