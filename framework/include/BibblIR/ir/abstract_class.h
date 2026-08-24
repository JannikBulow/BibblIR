// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_ABSTRACT_CLASS_H
#define BIBBLIR_IR_ABSTRACT_CLASS_H

#include "BibblIR/ir/field.h"
#include "BibblIR/ir/global.h"
#include "BibblIR/ir/method.h"

namespace bibblir {
    class BIBBLIR_EXPORT AbstractClass : public Global {
    public:
        using Global::Global;

        virtual std::string_view getModuleName() const = 0;
        virtual std::string_view getName() const = 0;

        virtual const std::vector<FieldPtr>& getFields() const = 0;
        virtual Field* getField(std::string_view name) const = 0;
        virtual Field* addField(Type* type, std::string name) = 0;

        virtual const std::vector<MethodPtr>& getMethods() const = 0;
        virtual Method* getMethod(std::string_view name) const = 0;
        virtual Method* addMethod(FunctionType* type, std::string name, Value* impl) = 0;
    };
}

#endif //BIBBLIR_IR_ABSTRACT_CLASS_H
