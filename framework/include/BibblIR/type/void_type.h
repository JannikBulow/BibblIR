// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_TYPE_VOID_TYPE_H
#define BIBBLIR_TYPE_VOID_TYPE_H

#include "BibblIR/type/type.h"

namespace bibblir {
    class BIBBLIR_EXPORT VoidType : public Type {
    public:
        VoidType();

        bool isVoidType() const override;
    };
}

#endif // BIBBLIR_TYPE_VOID_TYPE_H
