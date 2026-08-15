// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_INSTRUCTION_H
#define BIBBLIR_IR_INSTRUCTION_H

#include "BibblIR/ir/value.h"

namespace bibblir {
    class BasicBlock;

    class BIBBLIR_EXPORT Instruction : public Value {
    public:
        Instruction(Module& module, BasicBlock* parent) : Value(module), mParent(parent) {}

        BasicBlock* getParent() const { return mParent; }

        virtual void eraseFromParent();

    protected:
        BasicBlock* mParent;
    };
}

#endif //BIBBLIR_IR_INSTRUCTION_H
