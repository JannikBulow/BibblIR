// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_TYPE_STRING_TYPE_H
#define BIBBLIR_TYPE_STRING_TYPE_H

#include "BibblIR/type/type.h"

namespace bibblir {
    class BIBBLIR_EXPORT StringType : public Type {
    public:
        StringType();

        ID getID() const override;

        bool isStringType() const override;
    };
}

#endif //BIBBLIR_TYPE_STRING_TYPE_H
