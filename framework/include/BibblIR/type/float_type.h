// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_TYPE_FLOAT_TYPE_H
#define BIBBLIR_TYPE_FLOAT_TYPE_H

#include "BibblIR/type/type.h"

namespace bibblir {
    class BIBBLIR_EXPORT FloatType : public Type {
    public:
        FloatType(int bytes);

        ID getID() const override;

        bool isFloatType() const override;
    };
}

#endif //BIBBLIR_TYPE_FLOAT_TYPE_H
