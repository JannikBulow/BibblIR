// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_CONSTANT_CONSTANT_STRING_H
#define BIBBLIR_IR_CONSTANT_CONSTANT_STRING_H

#include "BibblIR/ir/value.h"

#include <string>

namespace bibblir {
    class BasicBlock;
    class BIBBLIR_EXPORT ConstantString : public Value {
        friend class CodegenVisitor;
        friend class IRBuilder;
        friend class PrintVisitor;
    public:
        std::string_view getValue() const;

        bool isConstant() const override;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;
    private:
        std::string mValue;

        ConstantString(Module& module, std::string value);
    };
}

#endif //BIBBLIR_IR_CONSTANT_CONSTANT_STRING_H
