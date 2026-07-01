// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_VISITOR_VISITOR_H
#define BIBBLIR_VISITOR_VISITOR_H

#include "BibblIR/api.h"

namespace bibblir {
    class Module;

    class Argument;
    class Function;
    class BasicBlock;

    class ConstantBoolean;
    class ConstantInt;

    class BinaryInstruction;
    class BranchInstruction;
    class CallInstruction;
    class LoadInstruction;
    class PhiInstruction;
    class ReturnInstruction;
    class UnaryInstruction;

    class BIBBLIR_EXPORT Visitor {
    public:
        virtual ~Visitor() = default;

        virtual void visit(Module& module) = 0;

        virtual void visit(Function& function) = 0;

        virtual void visit(BasicBlock& block) = 0;

        virtual void visit(Argument& arg) = 0;

        virtual void visit(ConstantBoolean& constant) = 0;
        virtual void visit(ConstantInt& constant) = 0;

        virtual void visit(BinaryInstruction& instruction) = 0;
        virtual void visit(BranchInstruction& instruction) = 0;
        virtual void visit(CallInstruction& instruction) = 0;
        virtual void visit(LoadInstruction& instruction) = 0;
        virtual void visit(PhiInstruction& instruction) = 0;
        virtual void visit(ReturnInstruction& instruction) = 0;
        virtual void visit(UnaryInstruction& instruction) = 0;
    };
}

#endif // BIBBLIR_VISITOR_VISITOR_H
