// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/type/boolean_type.h"
#include "BibblIR/type/function_type.h"
#include "BibblIR/type/integer_type.h"
#include "BibblIR/type/void_type.h"

#include <format>

namespace bibblir {
    BooleanType::BooleanType() : Type(1, "boolean") {
        mOperandSize = bibbleasm::OperandSize::Byte;
    }

    bool BooleanType::isBooleanType() const {
        return true;
    }

    FunctionType::FunctionType(Type* returnType, std::vector<Type*> argumentTypes)
        : Type(0, std::format("{}(", returnType->getName()))
        , mReturnType(returnType)
        , mArgumentTypes(std::move(argumentTypes)) {
        if (!mArgumentTypes.empty()) {
            for (size_t i = 0; i < mArgumentTypes.size() - 1; i++) {
                mName += mArgumentTypes[i]->getName();
                mName += ", ";
            }
            mName += mArgumentTypes.back()->getName();
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
        return static_cast<FunctionType*>(Type::GetFunctionType(returnType, argumentTypes));
    }

    IntegerType::IntegerType(int bytes)
        : Type(bytes, "") {
        switch (bytes) {
            case 1:
                mName = "byte";
                mOperandSize = bibbleasm::OperandSize::Byte;
                break;
            case 2:
                mName = "short";
                mOperandSize = bibbleasm::OperandSize::Short;
                break;
            case 4:
                mName = "int";
                mOperandSize = bibbleasm::OperandSize::Int;
                break;
            case 8:
                mName = "long";
                mOperandSize = bibbleasm::OperandSize::Long;
                break;
            default:
                mName = "int" + std::to_string(bytes);
                break;
        }
    }

    bool IntegerType::isIntegerType() const {
        return true;
    }

    VoidType::VoidType()
        : Type(0, "void") {}

    bool VoidType::isVoidType() const {
        return true;
    }
}