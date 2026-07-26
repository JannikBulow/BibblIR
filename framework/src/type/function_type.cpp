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

    Type* FunctionType::getReturnType() const {
        return mReturnType;
    }

    const std::vector<Type*>& FunctionType::getArgumentTypes() const {
        return mArgumentTypes;
    }

    Type::ID FunctionType::getID() const {
        return ID::Handle; // i guess this is the best type id? just pray this isn't called
    }

    bool FunctionType::isFunctionType() const {
        return true;
    }

    FunctionType* FunctionType::Create(Type* returnType, const std::vector<Type*>& argumentTypes) {
        return static_cast<FunctionType*>(GetFunctionType(returnType, argumentTypes));
    }
}
