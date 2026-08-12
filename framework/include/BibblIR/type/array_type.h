// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_TYPE_ARRAY_TYPE_H
#define BIBBLIR_TYPE_ARRAY_TYPE_H

#include "BibblIR/type/type.h"

namespace bibblir {
    class BIBBLIR_EXPORT ArrayType : public Type {
    public:
        explicit ArrayType(Type* elementType);

        Type* getElementType() const;

        ID getID() const override;

        bool isArrayType() const override;

    private:
        Type* mElementType;
    };
}

#endif //BIBBLIR_TYPE_ARRAY_TYPE_H
