// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_TYPE_CLASS_TYPE_H
#define BIBBLIR_TYPE_CLASS_TYPE_H

#include "BibblIR/type/type.h"

namespace bibblir {
    class BIBBLIR_EXPORT ClassType : public Type {
    public:
        ClassType(std::string moduleName, std::string name);

        std::string_view getModuleName() const;
        std::string_view getClassName() const;

        ID getID() const override;

        bool isClassType() const override;

    private:
        std::string mModuleName;
        std::string mClassName;
    };
}

#endif //BIBBLIR_TYPE_CLASS_TYPE_H
