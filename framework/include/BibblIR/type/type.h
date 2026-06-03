// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_TYPE_TYPE_H
#define BIBBLIR_TYPE_TYPE_H

#include "BibblIR/api.h"

#include <BibbleASM/instruction/operand.h>

#include <string>

namespace bibblir {
    class BIBBLIR_EXPORT Type {
    public:
        virtual ~Type() = default;

        size_t getSizeInBytes() const { return mSizeInBytes; }
        std::string_view getName() const { return mName; }
        bibbleasm::OperandSize getOperandSize() const { return mOperandSize; }

        virtual bool isVoidType() const { return false; }
        virtual bool isIntegerType() const { return false; }

        static Type* GetVoidType();
        static Type* GetIntegerType(int bytes);

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
