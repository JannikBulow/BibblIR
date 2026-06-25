// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/type/function_type.h"

#include <format>

namespace bibblir {
    FunctionType::FunctionType(Type* returnType, std::vector<Type*> argumentTypes)
        : Type(0, std::format("{}(", returnType->getName()))
        , mReturnType(returnType)
        , mArgumentTypes(std::move(argumentTypes)) {
        if (!mArgumentTypes.empty()) {
            for (size_t i = 0; i < mArgumentTypes.size() - 1; i++) {
                mName += mArgumentTypes[i]->getName();
                mName += ", ";
            }
            mName += mArgumentTypes[mArgumentTypes.size() - 1]->getName();
        }
        mName += ')';
    }

    bool FunctionType::isFunctionType() const {
        return true;
    }

    Type* FunctionType::getReturnType() const {
        return mReturnType;
    }

    const std::vector<Type*>& FunctionType::getArgumentTypes() const {
        return mArgumentTypes;
    }

    FunctionType* FunctionType::Create(Type* returnType, const std::vector<Type*>& argumentTypes) {
        return static_cast<FunctionType*>(GetFunctionType(returnType, argumentTypes));
    }
}
