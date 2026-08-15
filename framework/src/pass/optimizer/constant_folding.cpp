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

#include "BibblIR/pass/optimizer/constant_folding.h"

#include "BibblIR/module.h"

#include <cassert>

namespace bibblir {
    PassID ConstantFoldingPass::getId() const {
        return GetPassID<ConstantFoldingPass>();
    }

    std::vector<IRProperty> ConstantFoldingPass::getProvidedProperties() const {
        return {};
    }

    std::vector<IRProperty> ConstantFoldingPass::getRequiredProperties() const {
        return {};
    }

    void ConstantFoldingPass::run(Module& module) {
        module.accept(*this);
    }

    void ConstantFoldingPass::visit(Module& module) {
        for (auto& global : module.getGlobals()) {
            global->accept(*this);
        }
    }

    void ConstantFoldingPass::visit(Class& clas) {
        for (auto& field : clas.getFields()) {
            field->accept(*this);
        }

        for (auto& method : clas.getMethods()) {
            method->accept(*this);
        }
    }

    void ConstantFoldingPass::visit(Field& field) {

    }

    void ConstantFoldingPass::visit(Method& method) {

    }

    void ConstantFoldingPass::visit(Function& function) {
        for (auto& argument : function.getArguments()) {
            argument->accept(*this);
        }

        for (auto& bb : function.basicBlocks()) {
            bb->accept(*this);
        }
    }

    void ConstantFoldingPass::visit(ExternalFunction& function) {

    }

    void ConstantFoldingPass::visit(BasicBlock& block) {
        for (auto& value : block.getValues()) {
            value->accept(*this);
        }
    }

    void ConstantFoldingPass::visit(Argument& arg) {

    }

    void ConstantFoldingPass::visit(ConstantBoolean& constant) {
        constant.setConstantFoldedValue(constant.getValue());
    }

    void ConstantFoldingPass::visit(ConstantInt& constant) {
        constant.setConstantFoldedValue(constant.getValue());
    }

    void ConstantFoldingPass::visit(BinaryInstruction& instruction) {
        if (instruction.getLeft()->isConstantFolded() && instruction.getRight()->isConstantFolded()) {
            instruction.getLeft()->removeForceRegister();
            instruction.getRight()->removeForceRegister();

            if (instruction.getLeft()->getType()->isIntegerType()) {
                uintmax_t left = instruction.getLeft()->getConstantFoldedValue();
                uintmax_t right = instruction.getRight()->getConstantFoldedValue();
                uintmax_t value = 0;

                switch (instruction.getOperator()) {
                    case BinaryInstruction::ADD:
                        value = left + right;
                        break;
                    case BinaryInstruction::SUB:
                        value = left - right;
                        break;
                    case BinaryInstruction::MUL:
                        value = left * right;
                        break;
                    case BinaryInstruction::SDIV:
                        value = static_cast<intmax_t>(left) / static_cast<intmax_t>(right);
                        break;
                    case BinaryInstruction::UDIV:
                        value = left / right;
                        break;
                    case BinaryInstruction::SMOD:
                        value = static_cast<intmax_t>(left) % static_cast<intmax_t>(right);
                        break;
                    case BinaryInstruction::UMOD:
                        value = left % right;
                        break;
                    case BinaryInstruction::AND:
                        value = left & right;
                        break;
                    case BinaryInstruction::OR:
                        value = left | right;
                        break;
                    case BinaryInstruction::XOR:
                        value = left ^ right;
                        break;
                    case BinaryInstruction::SHL:
                        value = left << right;
                        break;
                    case BinaryInstruction::SHR:
                        value = left >> right;
                        break;
                    case BinaryInstruction::SAR: {
                        if (right == 0) {
                            value = left;
                        } else {
                            uintmax_t sign = left >> 63;
                            uintmax_t shifted = left >> right;
                            uint64_t extend = (left & (uint64_t{1} << 63)) ? (~uint64_t{0} << (64 - right)) : 0;
                            value = shifted | extend;
                        }
                        break;
                    }
                    case BinaryInstruction::EQ:
                        value = left == right;
                        break;
                    case BinaryInstruction::NE:
                        value = left != right;
                        break;
                    case BinaryInstruction::LT:
                        value = left < right;
                        break;
                    case BinaryInstruction::GT:
                        value = left > right;
                        break;
                    case BinaryInstruction::LE:
                        value = left <= right;
                        break;
                    case BinaryInstruction::GE:
                        value = left >= right;
                        break;
                }

                instruction.setConstantFoldedValue(value);
            } else if (instruction.getLeft()->getType()->isFloatType()) {
                double left = std::bit_cast<double>(instruction.getLeft()->getConstantFoldedValue());
                double right = std::bit_cast<double>(instruction.getLeft()->getConstantFoldedValue());
                double value = 0;

                switch (instruction.getOperator()) {
                    case BinaryInstruction::ADD:
                        value = left + right;
                        break;
                    case BinaryInstruction::SUB:
                        value = left - right;
                        break;
                    case BinaryInstruction::MUL:
                        value = left * right;
                        break;
                    case BinaryInstruction::SDIV:
                        value = left / right;
                        break;
                    case BinaryInstruction::UDIV:
                    case BinaryInstruction::SMOD:
                    case BinaryInstruction::UMOD:
                    case BinaryInstruction::AND:
                    case BinaryInstruction::OR:
                    case BinaryInstruction::XOR:
                    case BinaryInstruction::SHL:
                    case BinaryInstruction::SHR:
                    case BinaryInstruction::SAR:
                        assert(false);
                        break;
                    case BinaryInstruction::EQ:
                        value = left == right;
                        break;
                    case BinaryInstruction::NE:
                        value = left != right;
                        break;
                    case BinaryInstruction::LT:
                        value = left < right;
                        break;
                    case BinaryInstruction::GT:
                        value = left > right;
                        break;
                    case BinaryInstruction::LE:
                        value = left <= right;
                        break;
                    case BinaryInstruction::GE:
                        value = left >= right;
                        break;
                }

                instruction.setConstantFoldedValue(std::bit_cast<uintmax_t>(value));
            }
        }
    }

    void ConstantFoldingPass::visit(BranchInstruction& instruction) {
        if (instruction.getCondition() && instruction.getCondition()->isConstantFolded()) {
            instruction.getCondition()->removeForceRegister();

            uintmax_t value = instruction.getCondition()->getConstantFoldedValue();
            if (value) {
                std::erase(instruction.falseBranch()->predecessors(), instruction.getParent());
                std::erase(instruction.getParent()->successors(), instruction.falseBranch());
                instruction.falseBranch() = nullptr;
            } else {
                std::erase(instruction.trueBranch()->predecessors(), instruction.getParent());
                std::erase(instruction.getParent()->successors(), instruction.trueBranch());
                instruction.trueBranch() = nullptr;
            }
            instruction.setConstantFoldedValue(value);
        }
    }

    void ConstantFoldingPass::visit(CallInstruction& instruction) {

    }

    void ConstantFoldingPass::visit(GetElementInstruction& instruction) {

    }

    void ConstantFoldingPass::visit(GetMemberInstruction& instruction) {

    }

    void ConstantFoldingPass::visit(IntCastInstruction& instruction) {

    }

    void ConstantFoldingPass::visit(LoadInstruction& instruction) {

    }

    void ConstantFoldingPass::visit(NewInstruction& instruction) {

    }

    void ConstantFoldingPass::visit(PhiInstruction& instruction) {

    }

    void ConstantFoldingPass::visit(ReturnInstruction& instruction) {

    }

    void ConstantFoldingPass::visit(StoreInstruction& instruction) {

    }

    void ConstantFoldingPass::visit(UnaryInstruction& instruction) {
        if (instruction.getOperand()->isConstantFolded()) {
            instruction.getOperand()->removeForceRegister();

            if (instruction.getOperand()->getType()->isIntegerType()) {
                uintmax_t operand = instruction.getOperand()->getConstantFoldedValue();
                uintmax_t value = 0;

                switch (instruction.getOperator()) {
                    case UnaryInstruction::NEG:
                        value = -operand;
                        break;
                    case UnaryInstruction::ABS:
                        value = operand;
                        if (value < 0) value = -value;
                        break;
                    case UnaryInstruction::NOT:
                        value = ~operand;
                        break;
                }

                instruction.setConstantFoldedValue(value);
            } else if (instruction.getOperand()->getType()->isFloatType()) {
                double operand = std::bit_cast<double>(instruction.getOperand()->getConstantFoldedValue());
                double value = 0;

                switch (instruction.getOperator()) {
                    case UnaryInstruction::NEG:
                        value = -operand;
                        break;
                    case UnaryInstruction::ABS:
                        value = std::abs(operand);
                        break;
                    case UnaryInstruction::NOT:
                        assert(false);
                        break;
                }

                instruction.setConstantFoldedValue(std::bit_cast<uintmax_t>(value));
            }
        }
    }
}
