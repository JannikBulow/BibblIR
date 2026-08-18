// Copyright 2026 Jannik Laugmand Bülow

#include <BibbleBytecode/writer.h>

#include <BibblIR/ir/constant/constant_int.h>

#include <BibblIR/ir/instruction/binary_instruction.h>
#include <BibblIR/ir/instruction/call_instruction.h>
#include <BibblIR/ir/instruction/phi_instruction.h>

#include <BibblIR/ir/builder.h>
#include <BibblIR/ir/function.h>

#include <BibblIR/pass/codegen/codegen.h>

#include <BibblIR/pass/optimizer/constant_folding.h>
#include <BibblIR/pass/optimizer/peephole.h>

#include <BibblIR/pass/pass_manager.h>

#include <BibblIR/visitor/codegen_visitor.h>
#include <BibblIR/visitor/print_visitor.h>

#include <BibblIR/module.h>

#include <fstream>
#include <iostream>

#include "BibblIR/pass/regalloc/allocator.h"

using namespace bibblir;

int main() {
    Module module("Main");

    IRBuilder builder;

    Type* intType = Type::GetIntegerType(4);

    Function* mainFunc =
        Function::Create(module,
            FunctionType::Create(intType, {intType}),
            ".main");

    BasicBlock* entry = mainFunc->createBasicBlock("");
    builder.setInsertPoint(entry);

    builder.createReturn(
        builder.createAdd(
            builder.createConstantInt(34, intType),
            builder.createConstantInt(35, intType)
        )
    );


    PrintVisitor printVisitor(std::cout);
    module.accept(printVisitor);
    std::cout << "\n\n";


    PassRegistry passRegistry = PassRegistry::Default();
    PassManager passManager(passRegistry);

    //passManager.addPass(passRegistry.create(GetPassID<ConstantFoldingPass>()));
    passManager.addPass(passRegistry.create(GetPassID<CodegenPass>()));
    passManager.addPass(passRegistry.create(GetPassID<PeepholePass>()));

    passManager.buildPipeline().run(module);

    bibbleasm::Module builtModule = module.bytecodeModule().build();

    bibblebytecode::WritableByteBuffer buf;
    if (!bibblebytecode::writer::WriteModule(buf, builtModule.module())) {
        return 1;
    }

    std::ofstream output("Main.bmod", std::ios::binary);
    buf.emit(output);

    return 0;
}
