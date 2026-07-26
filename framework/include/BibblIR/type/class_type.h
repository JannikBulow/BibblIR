// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_TYPE_CLASS_TYPE_H
#define BIBBLIR_TYPE_CLASS_TYPE_H

#include "BibblIR/type/type.h"

namespace bibblir {
    class BIBBLIR_EXPORT ClassType : public Type {
    public:
        ClassType();

        ID getID() const override;

        bool isClassType() const override;
    };
}

#endif //BIBBLIR_TYPE_CLASS_TYPE_H
