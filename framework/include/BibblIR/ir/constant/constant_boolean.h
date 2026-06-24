// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_CONSTANT_CONSTANT_BOOLEAN_H
#define BIBBLIR_IR_CONSTANT_CONSTANT_BOOLEAN_H

#include "BibblIR/ir/value.h"

namespace bibblir {
    class BIBBLIR_EXPORT ConstantBoolean : public Value {
        friend class CodegenVisitor;
    public:
        static ConstantBoolean* Get(Module& module, bool value);

        bool isConstant() const override;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;
    private:
        bool mValue;

        ConstantBoolean(Module& module, bool value);
    };
}

#endif //BIBBLIR_IR_CONSTANT_CONSTANT_BOOLEAN_H
