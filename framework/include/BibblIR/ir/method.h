// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_METHOD_H
#define BIBBLIR_IR_METHOD_H

#include "BibblIR/ir/value.h"

#include "BibblIR/type/function_type.h"

namespace bibblir {
    class AbstractClass;

    class Method : public Value {
        friend class Class;
        friend class CodegenVisitor;
        friend class ExternalClass;
        friend class PrintVisitor;
    public:
        FunctionType* getFunctionType() const;

        bool isAbstract() const;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        AbstractClass* mParent;
        std::string mName;
        Value* mImpl;

        Method(AbstractClass* parent, FunctionType* type, std::string name, Value* impl);
    };

    using MethodPtr = std::unique_ptr<Method>;
}

#endif //BIBBLIR_IR_METHOD_H
