// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_CONSTANT_CONSTANT_INT_H
#define BIBBLIR_IR_CONSTANT_CONSTANT_INT_H

#include "BibblIR/ir/value.h"

#include <cstdint>

namespace bibblir {
    class BasicBlock;
    class BIBBLIR_EXPORT ConstantInt : public Value {
        friend class CodegenVisitor;
        friend class IRBuilder;
        friend class PrintVisitor;
    public:
        bool isConstant() const override;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;
    private:
        intmax_t mValue;

        ConstantInt(BasicBlock* parent, intmax_t value, Type* type);
    };
}

#endif //BIBBLIR_IR_CONSTANT_CONSTANT_INT_H
