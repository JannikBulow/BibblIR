// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_TYPE_HANDLE_TYPE_H
#define BIBBLIR_TYPE_HANDLE_TYPE_H

#include "BibblIR/type/type.h"

namespace bibblir {
    class BIBBLIR_EXPORT HandleType : public Type {
    public:
        HandleType();

        ID getID() const override;

        bool isHandleType() const override;
    };
}

#endif //BIBBLIR_TYPE_HANDLE_TYPE_H
