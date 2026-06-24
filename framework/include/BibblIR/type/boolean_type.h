// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_TYPE_BOOLEAN_TYPE_H
#define BIBBLIR_TYPE_BOOLEAN_TYPE_H

#include "BibblIR/type/type.h"

namespace bibblir {
    class BIBBLIR_EXPORT BooleanType : public Type {
    public:
        BooleanType();

        bool isBooleanType() const override;
    };
}

#endif //BIBBLIR_TYPE_BOOLEAN_TYPE_H
