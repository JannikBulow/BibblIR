// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/pass/codegen/codegen.h"

#include "BibblIR/visitor/codegen_visitor.h"

#include "BibblIR/module.h"

#include <iostream>

namespace bibblir {
    PassID CodegenPass::getId() const {
        return GetPassID<CodegenPass>();
    }

    std::vector<IRProperty> CodegenPass::getProvidedProperties() const {
        return {IRProperty::EmittedBytecode};
    }

    std::vector<IRProperty> CodegenPass::getRequiredProperties() const {
        return {IRProperty::NoCriticalEdges, IRProperty::CanonicalCFG, IRProperty::AllocatedRegisters};
    }

    void CodegenPass::run(Module& module) {
        CodegenVisitor visitor;
        module.accept(visitor);
        module.bytecodeModule() = visitor.stealBuilder();
    }
}
