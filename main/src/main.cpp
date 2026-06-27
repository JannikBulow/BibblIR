// Copyright 2026 Jannik Laugmand Bülow

#include <BibbleBytecode/writer.h>

#include <BibblIR/ir/constant/constant_int.h>

#include <BibblIR/ir/instruction/binary_instruction.h>
#include <BibblIR/ir/instruction/call_instruction.h>

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

    Type* intType = Type::GetIntegerType(4);

    Function* addFunc = Function::Create(module, FunctionType::Create(intType, {intType, intType}), "add");
    BasicBlock* addEntryBB = addFunc->createBasicBlock("");

    builder.setInsertPoint(addEntryBB);

    builder.createReturn(
        builder.createAdd(
            addFunc->getArgument(0),
            addFunc->getArgument(1)
        )
    );

    Function* mainFunc = Function::Create(module, FunctionType::Create(intType, {}), ".main");
    BasicBlock* mainEntryBB = mainFunc->createBasicBlock("");

    builder.setInsertPoint(mainEntryBB);

    builder.createReturn(
        builder.createCall(addFunc, {
            builder.createConstantInt(34, intType),
            builder.createConstantInt(33, intType)
        })
    );

    PrintVisitor printVisitor(std::cout);
    module.accept(printVisitor);

    CodegenVisitor codegenVisitor;
    module.accept(codegenVisitor);

    std::cout << "\n\n";
    codegenVisitor.printDisassembly(std::cout);

    bibbleasm::Module builtModule = codegenVisitor.buildModule();

    bibblebytecode::WritableByteBuffer buf;
    if (!bibblebytecode::writer::WriteModule(buf, builtModule.module())) {
        return 1;
    }

    std::ofstream output("Main.bmod", std::ios::binary);
    buf.emit(output);

    return 0;
}
