// Copyright 2026 Jannik Laugmand Bülow

#include <BibbleBytecode/writer.h>

#include <BibblIR/ir/constant/constant_int.h>

#include <BibblIR/ir/instruction/binary_instruction.h>
#include <BibblIR/ir/instruction/unary_instruction.h>

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

    Type* intType = Type::GetIntegerType(4);

    builder.setInsertPoint(mainEntryBB);
    builder.createReturn(
        builder.createAbs(
            builder.createNeg(
                builder.createAdd(
                    builder.createConstantInt(34, intType), builder.createConstantInt(35, intType)
                )
            )
        )
    );

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
