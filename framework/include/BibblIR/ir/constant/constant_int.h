// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_CONSTANT_CONSTANT_INT_H
#define BIBBLIR_IR_CONSTANT_CONSTANT_INT_H

#include "BibblIR/ir/value.h"

#include <cstdint>

namespace bibblir {
    class ConstantInt : public Value {
        friend class CodegenVisitor;
    public:
        static ConstantInt* Get(Module& module, intmax_t value, Type* type);

        bool isConstant() const override;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;
    private:
        intmax_t mValue;

        ConstantInt(Module& module, intmax_t value, Type* type);
    };
}

#endif //BIBBLIR_IR_CONSTANT_CONSTANT_INT_H
