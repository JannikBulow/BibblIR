// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_VISITOR_VISITOR_H
#define BIBBLIR_VISITOR_VISITOR_H

#include "BibblIR/api.h"

namespace bibblir {
    class Argument;
    class BasicBlock;
    class Function;
    class Module;

    class BIBBLIR_EXPORT Visitor {
    public:
        virtual ~Visitor() = default;

        virtual void visit(Module& module) = 0;

        virtual void visit(Function& function) = 0;
        virtual void visit(BasicBlock& block) = 0;

        virtual void visit(Argument& arg) = 0;
    };
}

#endif // BIBBLIR_VISITOR_VISITOR_H
