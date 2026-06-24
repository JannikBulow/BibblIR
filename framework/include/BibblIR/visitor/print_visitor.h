// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_VISITOR_PRINT_VISITOR_H
#define BIBBLIR_VISITOR_PRINT_VISITOR_H

#include "BibblIR/visitor/visitor.h"

#include <ostream>

namespace bibblir {
    class BIBBLIR_EXPORT PrintVisitor : public Visitor {
    public:
        explicit PrintVisitor(std::ostream& stream);

        void visit(Module& module) override;

        void visit(Function& function) override;

        void visit(BasicBlock& block) override;

        void visit(Argument& arg) override;

        void visit(ConstantBoolean& constant) override;
        void visit(ConstantInt& constant) override;

        void visit(ReturnInstruction& instruction) override;

    private:
        std::ostream& mStream;
    };
}

#endif //BIBBLIR_VISITOR_PRINT_VISITOR_H
