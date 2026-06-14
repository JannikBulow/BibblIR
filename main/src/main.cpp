// Copyright 2026 Jannik Laugmand Bülow

#include <BibbleBytecode/writer.h>

#include "BibblIR/ir/constant/constant_int.h"

#include <BibblIR/ir/builder.h>
#include <BibblIR/ir/function.h>

#include <BibblIR/visitor/codegen_visitor.h>
#include <BibblIR/visitor/print_visitor.h>

#include <BibblIR/module.h>

#include <fstream>
#include <iostream>

using namespace bibblir;

int main() {
    Module module("Main");

    IRBuilder builder;

    Function* mainFunc = Function::Create(module, FunctionType::Create(Type::GetIntegerType(4), {}), ".main");
    BasicBlock* mainEntryBB = mainFunc->createBasicBlock("");

    builder.setInsertPoint(mainEntryBB);
    builder.createReturn(ConstantInt::Get(module, 42, Type::GetIntegerType(4)));

    PrintVisitor printVisitor(std::cout);
    module.accept(printVisitor);

    CodegenVisitor codegenVisitor;
    module.accept(codegenVisitor);

    bibbleasm::Module builtModule = codegenVisitor.buildModule();

    bibblebytecode::WritableByteBuffer buf;
    if (!bibblebytecode::writer::WriteModule(buf, builtModule.module())) {
        return 1;
    }

    std::ofstream output("Main.bmod", std::ios::binary);
    buf.emit(output);

    return 0;
}
