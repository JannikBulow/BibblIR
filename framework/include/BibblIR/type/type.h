// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_TYPE_TYPE_H
#define BIBBLIR_TYPE_TYPE_H

#include "BibblIR/api.h"

#include <BibbleASM/instruction/operand.h>

#include <string>

namespace bibblir {
    class BIBBLIR_EXPORT Type {
    public:
        enum class ID : uint8_t {
            Byte = 0,
            UByte = 1,
            Short = 2,
            UShort = 3,
            Int = 4,
            UInt = 5,
            Long = 6,
            ULong = 7,
            Float = 8,
            Double = 9,

            Handle = 10,

            Reference = 11, // instance, array, string, future
        };

        virtual ~Type() = default;

        size_t getSizeInBytes() const { return mSizeInBytes; }
        std::string_view getName() const { return mName; }
        bibbleasm::OperandSize getOperandSize() const { return mOperandSize; }

        virtual ID getID() const = 0;
        uint8_t getIDByte() const { return static_cast<uint8_t>(getID()); }

        virtual bool isVoidType() const { return false; }
        virtual bool isIntegerType() const { return false; }
        virtual bool isBooleanType() const { return false; }
        virtual bool isFunctionType() const { return false; }
        virtual bool isArrayType() const { return false; }
        virtual bool isClassType() const { return false; }

        static Type* GetVoidType();
        static Type* GetIntegerType(int bytes);
        static Type* GetBooleanType();
        static Type* GetFunctionType(Type* returnType, const std::vector<Type*>& argumentTypes);
        static Type* GetArrayType(Type* elementType);
        static Type* GetClassType(std::string moduleName, std::string name);

    protected:
        Type(size_t sizeInBytes, std::string name)
            : mSizeInBytes(sizeInBytes)
            , mName(std::move(name))
            , mOperandSize(bibbleasm::OperandSize::Unknown) {}

        size_t mSizeInBytes;
        std::string mName;

        bibbleasm::OperandSize mOperandSize;
    };
}

#endif // BIBBLIR_TYPE_TYPE_H
