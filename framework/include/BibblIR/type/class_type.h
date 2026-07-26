// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_TYPE_CLASS_TYPE_H
#define BIBBLIR_TYPE_CLASS_TYPE_H

#include "BibblIR/type/type.h"

namespace bibblir {
    class BIBBLIR_EXPORT ClassType : public Type {
    public:
        explicit ClassType(std::vector<Type*> fields);

        bool isClassType() const override;

        const std::vector<Type*>& getFields() const;
        Type* getField(unsigned int index) const;

    private:
        std::vector<Type*> mFields;
    };
}

#endif //BIBBLIR_TYPE_CLASS_TYPE_H
