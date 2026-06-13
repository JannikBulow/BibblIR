// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/return_instruction.h"

#include "BibblIR/ir/function.h"

#include "BibblIR/visitor/print_visitor.h"

#include "BibblIR/module.h"

#include <format>

namespace bibblir {
    PrintVisitor::PrintVisitor(std::ostream& stream)
        : mStream(stream) {}

    void PrintVisitor::visit(Module& module) {
        const std::vector<GlobalPtr>& globals = module.getGlobals();

        for (const GlobalPtr& global : globals) {
            if (auto func = dynamic_cast<Function*>(global.get())) {
                func->orderBasicBlocks();
            }
        }

        mStream << std::format("module \"{}\"", module.getName());

        for (const GlobalPtr& global : globals) {
            global->accept(*this);
        }
    }

    void PrintVisitor::visit(Function& function) {
        mStream << std::format("\n\nfunction \"{}\"(", function.mName);
        if (!function.mArguments.empty()) {
            for (int i = 0; i < function.mArguments.size() - 1; i++) {
                mStream << std::format("{} {}, ", function.mArguments[i]->mType->getName(), function.mArguments[i ]->mName);
            }
            mStream << std::format("{} {}", function.mArguments.back()->mType->getName(), function.mArguments.back()->mName);
        }
        mStream << std::format(") -> {}\n", function.getFunctionType()->getReturnType()->getName());

        mStream << "{\n";
        for (const BasicBlockPtr& bb : function.mBasicBlockList) {
            bb->accept(*this);
        }
        mStream << "}";
    }

    void PrintVisitor::visit(BasicBlock& block) {
        mStream << std::format("  {}:\n", block.mName);
        for (const ValuePtr& value : block.mValueList) {
            mStream << "    ";
            value->accept(*this);
            mStream << "\n";
        }
    }

    void PrintVisitor::visit(Argument& arg) {}

    void PrintVisitor::visit(ReturnInstruction& instruction) {
        mStream << "return ";
        if (instruction.mReturnValue) {
            mStream << instruction.mReturnValue->identifier();
        } else {
            mStream << "void";
        }
    }
}
