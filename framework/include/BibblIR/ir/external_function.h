// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_EXTERNAL_FUNCTION_H
#define BIBBLIR_IR_EXTERNAL_FUNCTION_H

#include "BibblIR/ir/global.h"

#include "BibblIR/type/function_type.h"

namespace bibblir {
    class BIBBLIR_EXPORT ExternalFunction : public Global {
        friend class CodegenVisitor;
        friend class PrintVisitor;
        friend class RegAlloc;
    public:
        static ExternalFunction* Create(Module& module, FunctionType* type, std::string moduleName, std::string name);

        FunctionType* getFunctionType() const;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        std::string mModuleName;
        std::string mName;

        ExternalFunction(Module& module, FunctionType* type, std::string moduleName, std::string name);
    };
}

#endif //BIBBLIR_IR_EXTERNAL_FUNCTION_H
