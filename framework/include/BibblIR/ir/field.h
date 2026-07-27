// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_FIELD_H
#define BIBBLIR_IR_FIELD_H

#include "BibblIR/ir/value.h"

namespace bibblir {
    class Class;

    class Field : public Value {
        friend class Class;
        friend class CodegenVisitor;
        friend class PrintVisitor;
    public:
        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        Class* mParent;
        std::string mName;

        Field(Class* parent, Type* type, std::string name);
    };

    using FieldPtr = std::unique_ptr<Field>;
}

#endif //BIBBLIR_IR_FIELD_H
