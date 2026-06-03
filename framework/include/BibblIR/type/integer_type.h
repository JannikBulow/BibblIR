// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_TYPE_INTEGER_TYPE_H
#define BIBBLIR_TYPE_INTEGER_TYPE_H

#include "BibblIR/type/type.h"

namespace bibblir {
    class BIBBLIR_EXPORT IntegerType : public Type {
    public:
        IntegerType(int bytes);

        bool isIntegerType() const override;
    };
}

#endif // BIBBLIR_TYPE_INTEGER_TYPE_H
