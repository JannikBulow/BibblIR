// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_VISITOR_CODEGEN_VISITOR_H
#define BIBBLIR_VISITOR_CODEGEN_VISITOR_H

#include "BibblIR/visitor/visitor.h"

#include <BibbleASM/codegen/builder/module_builder.h>

#include <BibbleASM/instruction/builder.h>

#include <unordered_map>

namespace bibblir {
    class BIBBLIR_EXPORT CodegenVisitor : public Visitor {
    public:
        bibbleasm::Module buildModule();

        void visit(Module& module) override;

        void visit(Function& function) override;

        void visit(BasicBlock& block) override;

        void visit(Argument& arg) override;

        void visit(ConstantBoolean& constant) override;
        void visit(ConstantInt& constant) override;

        void visit(BinaryInstruction& instruction) override;
        void visit(PhiInstruction& instruction) override;
        void visit(ReturnInstruction& instruction) override;
        void visit(UnaryInstruction& instruction) override;

    private:
        bibbleasm::ModuleBuilder mBuilder;
        bibbleasm::InstructionBuilder* mInstBuilder = nullptr;

        std::unordered_map<std::string, bibbleasm::ConstantIndex> mStringConstants;

        bibbleasm::ConstantIndex getStringConstant(const std::string& str);
    };
}

#endif //BIBBLIR_VISITOR_CODEGEN_VISITOR_H
