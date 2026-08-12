// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/constant/constant_boolean.h"
#include "BibblIR/ir/constant/constant_int.h"

#include "BibblIR/ir/instruction/binary_instruction.h"
#include "BibblIR/ir/instruction/branch_instruction.h"
#include "BibblIR/ir/instruction/call_instruction.h"
#include "BibblIR/ir/instruction/getelement_instruction.h"
#include "BibblIR/ir/instruction/getmember_instruction.h"
#include "BibblIR/ir/instruction/int_cast_instruction.h"
#include "BibblIR/ir/instruction/load_instruction.h"
#include "BibblIR/ir/instruction/new_instruction.h"
#include "BibblIR/ir/instruction/phi_instruction.h"
#include "BibblIR/ir/instruction/return_instruction.h"
#include "BibblIR/ir/instruction/store_instruction.h"
#include "BibblIR/ir/instruction/unary_instruction.h"

#include "BibblIR/ir/class.h"
#include "BibblIR/ir/external_function.h"
#include "BibblIR/ir/function.h"

#include "BibblIR/visitor/print_visitor.h"

#include "BibblIR/module.h"

#include <format>

namespace bibblir {
    PrintVisitor::PrintVisitor(std::ostream& stream)
        : mStream(stream) {}

    void PrintVisitor::visit(Module& module) {
        const std::vector<GlobalPtr>& globals = module.getGlobals();

        mStream << std::format("module \"{}\"", module.getName());

        for (const GlobalPtr& global : globals) {
            global->accept(*this);
        }
    }

    void PrintVisitor::visit(Class& clas) {
        mStream << std::format("\n\nclass \"{}\" {{\n", clas.mName);

        for (const auto& field : clas.mFields) {
            field->accept(*this);
        }

        for (const auto& method : clas.mMethods) {
            method->accept(*this);
        }

        mStream << "}";
    }

    void PrintVisitor::visit(Field& field) {
        mStream << std::format("    {} \"{}\"\n", field.getType()->getName(), field.mName);
    }

    void PrintVisitor::visit(Method& method) {
        mStream << std::format("    virtual \"{}\" (", method.mName);
        if (!method.getFunctionType()->getArgumentTypes().empty()) {
            for (int i = 0; i < method.getFunctionType()->getArgumentTypes().size() - 1; i++) {
                mStream << std::format("{}, ", method.getFunctionType()->getArgumentTypes()[i]->getName());
            }
            mStream << method.getFunctionType()->getArgumentTypes().back()->getName();
        } else {
            mStream << "void";
        }
        mStream << std::format(") . {} = {}\n", method.getFunctionType()->getReturnType()->getName(), method.mImpl ? method.mImpl->identifier() : "null");
    }

    void PrintVisitor::visit(Function& function) {
        mStream << std::format("\n\nfunction \"{}\" (", function.mName);
        if (!function.mArguments.empty()) {
            for (int i = 0; i < function.mArguments.size() - 1; i++) {
                mStream << std::format("{} {}, ", function.mArguments[i]->mType->getName(), function.mArguments[i ]->identifier());
            }
            mStream << std::format("{} {}", function.mArguments.back()->mType->getName(), function.mArguments.back()->identifier());
        } else {
            mStream << "void";
        }
        mStream << std::format(") -> {}\n", function.getFunctionType()->getReturnType()->getName());

        mStream << "{\n";
        for (const BasicBlockPtr& bb : function.mBasicBlockList) {
            bb->accept(*this);
        }
        mStream << "}";
    }

    void PrintVisitor::visit(ExternalFunction& function) {
        mStream << std::format("\n\nimport function \"{}\"::\"{}\" (", function.mModuleName, function.mName);
        if (!function.getFunctionType()->getArgumentTypes().empty()) {
            for (int i = 0; i < function.getFunctionType()->getArgumentTypes().size() - 1; i++) {
                mStream << std::format("{}, ", function.getFunctionType()->getArgumentTypes()[i]->getName());
            }
            mStream << function.getFunctionType()->getArgumentTypes().back()->getName();
        } else {
            mStream << "void";
        }
        mStream << std::format(") -> {};", function.getFunctionType()->getReturnType()->getName());
    }

    void PrintVisitor::visit(BasicBlock& block) {
        if (block.exists()) {
            mStream << std::format("  {}:\n", block.mName);
            for (const ValuePtr& value : block.mValueList) {
                mStream << "    ";
                value->accept(*this);
                if (value->mInterval.first != -1 && value->mInterval.second != -1) {
                    mStream << std::format("    [{}, {}]", value->mInterval.first, value->mInterval.second);
                }
                mStream << "\n";
            }
        }
    }

    void PrintVisitor::visit(Argument& arg) {}

    void PrintVisitor::visit(ConstantBoolean& constant) {
        if (constant.mForceRegister) {
            mStream << std::format("{} = {} {}", constant.identifier(), constant.mType->getName(), constant.mValue);
        }
    }

    void PrintVisitor::visit(ConstantInt& constant) {
        if (constant.mForceRegister) {
            mStream << std::format("{} = {} {}", constant.identifier(), constant.mType->getName(), constant.mValue);
        }
    }

    void PrintVisitor::visit(BinaryInstruction& instruction) {
        std::string operatorSymbol;
        switch (instruction.getOperator()) {
            case BinaryInstruction::ADD:
                operatorSymbol = "+";
                break;
            case BinaryInstruction::SUB:
                operatorSymbol = "-";
                break;
            case BinaryInstruction::MUL:
                operatorSymbol = "*";
                break;
            case BinaryInstruction::SDIV:
                operatorSymbol = "/";
                break;
            case BinaryInstruction::UDIV:
                operatorSymbol = "/";
                break;
            case BinaryInstruction::SMOD:
                operatorSymbol = "%";
                break;
            case BinaryInstruction::UMOD:
                operatorSymbol = "%";
                break;
            case BinaryInstruction::AND:
                operatorSymbol = "&";
                break;
            case BinaryInstruction::OR:
                operatorSymbol = "|";
                break;
            case BinaryInstruction::XOR:
                operatorSymbol = "^";
                break;
            case BinaryInstruction::SHL:
                operatorSymbol = "<<";
                break;
            case BinaryInstruction::SHR:
                operatorSymbol = ">>>";
                break;
            case BinaryInstruction::SAR:
                operatorSymbol = ">>";
                break;
            case BinaryInstruction::EQ:
                operatorSymbol = "==";
                break;
            case BinaryInstruction::NE:
                operatorSymbol = "!=";
                break;
            case BinaryInstruction::LT:
                operatorSymbol = "<";
                break;
            case BinaryInstruction::GT:
                operatorSymbol = ">";
                break;
            case BinaryInstruction::LE:
                operatorSymbol = "<=";
                break;
            case BinaryInstruction::GE:
                operatorSymbol = ">=";
                break;
        }

        mStream << std::format("{} = {} {} {}", instruction.getName(instruction.mValueId), instruction.mLeft->identifier(), operatorSymbol, instruction.mRight->identifier());
    }

    void PrintVisitor::visit(BranchInstruction& instruction) {
        if (!instruction.mFalseBranch) {
            mStream << std::format("branch {}", instruction.mTrueBranch->identifier());
        } else {
            mStream << std::format("branch if {} ? {} : {}", instruction.mCondition->identifier(), instruction.mTrueBranch->identifier(), instruction.mFalseBranch->identifier());
        }
    }

    void PrintVisitor::visit(CallInstruction& instruction) {
        mStream << std::format("{} = call {}(", instruction.identifier(), instruction.mCallee->identifier());
        if (!instruction.mParameters.empty()) {
            for (int i = 0; i < instruction.mParameters.size() - 1; i++) {
                mStream << std::format("{}, ", instruction.mParameters[i]->identifier());
            }
            mStream << instruction.mParameters.back()->identifier();
        }
        mStream << ')';
    }

    void PrintVisitor::visit(GetElementInstruction& instruction) {
        mStream << std::format("{} = getelement {}[{}]", instruction.identifier(), instruction.mArray->identifier(), instruction.mIndex->identifier());
    }

    void PrintVisitor::visit(GetMemberInstruction& instruction) {
        mStream << std::format("{} = getmember {}.{}", instruction.identifier(), instruction.mObject->identifier(), instruction.mMember->identifier());
    }

    void PrintVisitor::visit(IntCastInstruction& instruction) {
        mStream << std::format("{} = intcast {} -> {}", instruction.identifier(), instruction.mValue->identifier(), instruction.mType->getName());
    }

    void PrintVisitor::visit(LoadInstruction& instruction) {
        mStream << std::format("{} = load {}", instruction.identifier(), instruction.mVariable->identifier());
    }

    void PrintVisitor::visit(NewInstruction& instruction) {
        mStream << std::format("{} = new {}", instruction.identifier(), instruction.getType()->getName());
    }

    void PrintVisitor::visit(PhiInstruction& instruction) {
        mStream << std::format("{} = phi [", instruction.getName(instruction.mValueId));
        for (size_t i = 0; i < instruction.mIncoming.size() - 1; i++) {
            mStream << std::format("{} : {}, ", instruction.mIncoming[i].first->identifier(), instruction.mIncoming[i].second->identifier());
        }
        mStream << std::format("{} : {} ]", instruction.mIncoming.back().first->identifier(), instruction.mIncoming.back().second->identifier());
    }

    void PrintVisitor::visit(ReturnInstruction& instruction) {
        mStream << "return ";
        if (instruction.mReturnValue) {
            mStream << instruction.mReturnValue->identifier();
        } else {
            mStream << "void";
        }
    }

    void PrintVisitor::visit(StoreInstruction& instruction) {
        mStream << std::format("store {} -> {}", instruction.mValue->identifier(), instruction.mVariable->identifier());
    }

    void PrintVisitor::visit(UnaryInstruction& instruction) {
        std::string operatorSymbol;
        switch (instruction.getOperator()) {
            case UnaryInstruction::NEG:
                operatorSymbol = "-";
                break;
            case UnaryInstruction::ABS:
                operatorSymbol = "abs ";
                break;
            case UnaryInstruction::NOT:
                operatorSymbol = "~";
                break;
        }

        mStream << std::format("{} = {}{}", instruction.getName(instruction.mValueId), operatorSymbol, instruction.mOperand->identifier());
    }
}
