// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_EXTERNAL_CLASS_H
#define BIBBLIR_EXTERNAL_CLASS_H

#include "BibblIR/ir/abstract_class.h"
#include "BibblIR/ir/field.h"
#include "BibblIR/ir/method.h"

namespace bibblir {
    class BIBBLIR_EXPORT ExternalClass : public AbstractClass {
    public:
        static ExternalClass* Create(Module& module, std::string moduleName, std::string name);

        std::string_view getModuleName() const override;
        std::string_view getName() const override;

        const std::vector<FieldPtr>& getFields() const override;
        Field* getField(std::string_view name) const override;
        Field* addField(Type* type, std::string name) override;

        const std::vector<MethodPtr>& getMethods() const override;
        Method* getMethod(std::string_view name) const override;
        Method* addMethod(FunctionType* type, std::string name, Value* impl) override;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        std::string mModuleName;
        std::string mName;
        std::vector<FieldPtr> mFields;
        std::vector<MethodPtr> mMethods;

        ExternalClass(Module& module, std::string moduleName, std::string name);
    };
}

#endif //BIBBLIR_EXTERNAL_CLASS_H
