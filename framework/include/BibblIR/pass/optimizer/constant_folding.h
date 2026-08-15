// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_PASS_OPTIMIZER_CONSTANT_FOLDING_H
#define BIBBLIR_PASS_OPTIMIZER_CONSTANT_FOLDING_H

#include "BibblIR/pass/pass.h"

#include "BibblIR/visitor/visitor.h"

namespace bibblir {
    class BIBBLIR_EXPORT ConstantFoldingPass : public Pass, Visitor {
    public:
        PassID getId() const override;

        std::vector<IRProperty> getProvidedProperties() const override;
        std::vector<IRProperty> getRequiredProperties() const override;

        void run(Module& module) override;

        void visit(Module& module) override;

        void visit(Class& clas) override;
        void visit(Field& field) override;
        void visit(Method& method) override;

        void visit(Function& function) override;

        void visit(ExternalFunction& function) override;

        void visit(BasicBlock& block) override;

        void visit(Argument& arg) override;

        void visit(ConstantBoolean& constant) override;
        void visit(ConstantInt& constant) override;

        void visit(BinaryInstruction& instruction) override;
        void visit(BranchInstruction& instruction) override;
        void visit(CallInstruction& instruction) override;
        void visit(GetElementInstruction& instruction) override;
        void visit(GetMemberInstruction& instruction) override;
        void visit(IntCastInstruction& instruction) override;
        void visit(LoadInstruction& instruction) override;
        void visit(NewInstruction& instruction) override;
        void visit(PhiInstruction& instruction) override;
        void visit(ReturnInstruction& instruction) override;
        void visit(StoreInstruction& instruction) override;
        void visit(UnaryInstruction& instruction) override;
    };
}

#endif //BIBBLIR_PASS_OPTIMIZER_CONSTANT_FOLDING_H
