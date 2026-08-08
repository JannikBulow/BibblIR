// Copyright 2026 Jannik Laugmand Bülow

#include <BibbleBytecode/writer.h>

#include <BibblIR/ir/constant/constant_int.h>

#include <BibblIR/ir/instruction/binary_instruction.h>
#include <BibblIR/ir/instruction/call_instruction.h>
#include <BibblIR/ir/instruction/phi_instruction.h>

#include <BibblIR/ir/builder.h>
#include <BibblIR/ir/function.h>

#include <BibblIR/pass/codegen/codegen.h>

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
    BasicBlock* a = mainFunc->createBasicBlock("");
    BasicBlock* b = mainFunc->createBasicBlock("");
    BasicBlock* merge = mainFunc->createBasicBlock("");

    builder.setInsertPoint(entry);
    builder.createBr(a);

    builder.setInsertPoint(a);
    builder.createCondBr(
        builder.createCmpNE(mainFunc->getArgument(0), builder.createConstantInt(0, intType)),
        b,
        merge
    );

    builder.setInsertPoint(b);
    builder.createBr(merge);

    builder.setInsertPoint(merge);

    PhiInstruction* phi = builder.createPhi(intType);
    phi->addIncoming(builder.createConstantInt(10, intType), a);
    phi->addIncoming(builder.createConstantInt(20, intType), b);

    builder.createReturn(phi);


    PrintVisitor printVisitor(std::cout);
    module.accept(printVisitor);
    std::cout << "\n\n";


    PassRegistry passRegistry = PassRegistry::Default();
    PassManager passManager(passRegistry);

    passManager.addPass(passRegistry.create(GetPassID<CodegenPass>()));

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
