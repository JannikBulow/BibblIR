// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/bytecode/utils.h"

#include "BibblIR/ir/constant/constant_boolean.h"
#include "BibblIR/ir/constant/constant_int.h"

#include "BibblIR/ir/instruction/binary_instruction.h"
#include "BibblIR/ir/instruction/branch_instruction.h"
#include "BibblIR/ir/instruction/call_instruction.h"
#include "BibblIR/ir/instruction/phi_instruction.h"
#include "BibblIR/ir/instruction/return_instruction.h"
#include "BibblIR/ir/instruction/unary_instruction.h"

#include "BibblIR/ir/function.h"

#include "BibblIR/optimizer/regalloc/allocator.h"

#include "BibblIR/visitor/codegen_visitor.h"

#include "BibblIR/module.h"

#include <cassert>

namespace bibblir {
    bibbleasm::Module CodegenVisitor::buildModule() {
        bibbleasm::Module module = mBuilder.build();
        mBuilder = {}; // reset
        return module;
    }

    void CodegenVisitor::printDisassembly(std::ostream& stream) {
        const auto& functions = mBuilder.functions();
        for (size_t i = 0; i < functions.size() - 1; i++) {
            stream << "function " << functions[i]->getName() << ":\n";
            stream << functions[i]->assembler().disassemble();
            stream << "\n\n";
        }
        stream << "function " << functions.back()->getName() << ":\n";
        stream << functions.back()->assembler().disassemble();
    }

    void CodegenVisitor::visit(Module& module) {
        mModuleName = &module.getName();

        mBuilder.setVersion(1);
        mBuilder.setName(getStringConstant(module.getName()));

        const std::vector<GlobalPtr>& globals = module.getGlobals();

        for (const GlobalPtr& global : globals) {
            if (auto func = dynamic_cast<Function*>(global.get())) {
                func->orderBasicBlocks();
            }
        }

        RegAlloc regalloc;
        for (const GlobalPtr& global : globals) {
            if (auto func = dynamic_cast<Function*>(global.get())) {
                regalloc.assignVRegs(func);
            }
        }

        for (const GlobalPtr& global : globals) {
            global->accept(*this);
        }

        mModuleName = nullptr;
    }

    void CodegenVisitor::visit(Function& function) {
        uint16_t flags = 0;
        if (function.getBasicBlockCount() == 0) {
            flags |= bibblebytecode::FUNC_NATIVE; // i might make a dedicated flag system for the Function class in the future, but rn an empty function is native
        }

        bibbleasm::FunctionBuilder& functionBuilder = mBuilder.addFunction(getStringConstant(function.mName), flags, function.mRegisterCount, function.mArguments.size());
        bibbleasm::InstructionBuilder instructionBuilder(functionBuilder.assembler());
        mInstBuilder = &instructionBuilder;

        for (const BasicBlockPtr& bb : function.basicBlocks()) {
            bb->mEmittedValue = bibbleasm::Label(bb->mName);
        }

        for (const ArgumentPtr& argument : function.mArguments) {
            argument->accept(*this);
        }

        for (const BasicBlockPtr& bb : function.basicBlocks()) {
            bb->accept(*this);
        }

        for (const BasicBlockPtr& bb : function.basicBlocks()) {
            for (auto phi : bb->mPhis) {
                phi->accept(*this);
            }
        }

        mInstBuilder = nullptr;
    }

    void CodegenVisitor::visit(BasicBlock& block) {
        if (block.exists()) {
            mInstBuilder->label(block.mName);
            for (const ValuePtr& value : block.mValueList) {
                if (auto phi = dynamic_cast<PhiInstruction*>(value.get())) {
                    phi->setEmittedValue();
                } else {
                    value->accept(*this);
                }
            }
        }
    }

    void CodegenVisitor::visit(Argument& arg) {
        arg.mEmittedValue = bibbleasm::Register(arg.mIndex);
    }

    void CodegenVisitor::visit(ConstantBoolean& constant) {
        constant.mEmittedValue = bibbleasm::Immediate(bibbleasm::OperandSize::Byte, constant.mValue);

        if (constant.mForceRegister) {
            bibbleasm::Register reg = constant.mVReg->toOperand();
            bytecode::Move(*mInstBuilder, reg, *constant.mEmittedValue);
            constant.mEmittedValue = reg;
        }
    }

    void CodegenVisitor::visit(ConstantInt& constant) {
        constant.mEmittedValue = bibbleasm::Immediate(constant.mValue);

        if (constant.mForceRegister) {
            bibbleasm::Register reg = constant.mVReg->toOperand();
            bytecode::Move(*mInstBuilder, reg, *constant.mEmittedValue);
            constant.mEmittedValue = reg;
        }
    }

    void CodegenVisitor::visit(BinaryInstruction& instruction) {
        bibbleasm::Register leftReg = std::get<bibbleasm::Register>(*instruction.mLeft->mEmittedValue);
        bibbleasm::Register rightReg = std::get<bibbleasm::Register>(*instruction.mRight->mEmittedValue);

        bibbleasm::Register dst = instruction.mVReg->toOperand();

        if (instruction.mLeft->getType()->isIntegerType()) {
            switch (instruction.mOperator) {
                case BinaryInstruction::ADD:
                    mInstBuilder->add(dst, leftReg, rightReg);
                    break;
                case BinaryInstruction::SUB:
                    mInstBuilder->sub(dst, leftReg, rightReg);
                    break;
                case BinaryInstruction::MUL:
                    mInstBuilder->mul(dst, leftReg, rightReg);
                    break;
                case BinaryInstruction::SDIV:
                    mInstBuilder->sdiv(dst, leftReg, rightReg);
                    break;
                case BinaryInstruction::UDIV:
                    mInstBuilder->udiv(dst, leftReg, rightReg);
                    break;
                case BinaryInstruction::SMOD:
                    mInstBuilder->smod(dst, leftReg, rightReg);
                    break;
                case BinaryInstruction::UMOD:
                    mInstBuilder->umod(dst, leftReg, rightReg);
                    break;
                case BinaryInstruction::AND:
                    mInstBuilder->and_(dst, leftReg, rightReg);
                    break;
                case BinaryInstruction::OR:
                    mInstBuilder->or_(dst, leftReg, rightReg);
                    break;
                case BinaryInstruction::XOR:
                    mInstBuilder->xor_(dst, leftReg, rightReg);
                    break;
                case BinaryInstruction::SHL:
                    mInstBuilder->shl(dst, leftReg, rightReg);
                    break;
                case BinaryInstruction::SHR:
                    mInstBuilder->shr(dst, leftReg, rightReg);
                    break;
                case BinaryInstruction::SAR:
                    mInstBuilder->sar(dst, leftReg, rightReg);
                    break;
                case BinaryInstruction::EQ:
                case BinaryInstruction::NE:
                case BinaryInstruction::LT:
                case BinaryInstruction::GT:
                case BinaryInstruction::LE:
                case BinaryInstruction::GE:
                    mInstBuilder->icmp(dst, leftReg, rightReg);
                    break;
            }
        }

        instruction.mEmittedValue = dst; // the conditional branch codegen could simply check if its condition is a BinaryInstruction, then check the operator
    }

    void CodegenVisitor::visit(BranchInstruction& instruction) {
        assert(instruction.mParent->endId() == static_cast<bibbleasm::InstructionId>(-1));
        instruction.mParent->endId() = mInstBuilder->assembler().getLastInstructionId();

        if (!instruction.mFalseBranch) {
            bytecode::Jump(*mInstBuilder, *instruction.mTrueBranch->mEmittedValue);
        } else {
            if (auto binary = dynamic_cast<BinaryInstruction*>(instruction.mCondition)) {
                bytecode::CondType condType;
                switch (binary->mOperator) {
                    case BinaryInstruction::EQ:
                        condType = bytecode::CondType::EQ;
                        break;
                    case BinaryInstruction::NE:
                        condType = bytecode::CondType::NE;
                        break;
                    case BinaryInstruction::LT:
                        condType = bytecode::CondType::LT;
                        break;
                    case BinaryInstruction::GT:
                        condType = bytecode::CondType::GT;
                        break;
                    case BinaryInstruction::LE:
                        condType = bytecode::CondType::LE;
                        break;
                    case BinaryInstruction::GE:
                        condType = bytecode::CondType::GE;
                        break;

                    default:
                        assert(false);
                        break;
                }

                bytecode::CondJump(*mInstBuilder, condType, *instruction.mCondition->mEmittedValue,  *instruction.mTrueBranch->mEmittedValue);
                bytecode::Jump(*mInstBuilder, *instruction.mFalseBranch->mEmittedValue);
            } else if (std::holds_alternative<bibbleasm::Immediate>(*instruction.mCondition->mEmittedValue)) {
                bibbleasm::Immediate imm = std::get<bibbleasm::Immediate>(*instruction.mCondition->mEmittedValue);
                if (imm.value) {
                    bytecode::Jump(*mInstBuilder, *instruction.mTrueBranch->mEmittedValue);
                } else {
                    bytecode::Jump(*mInstBuilder, *instruction.mFalseBranch->mEmittedValue);
                }
            }
        }
    }

    void CodegenVisitor::visit(CallInstruction& instruction) {
        if (!instruction.mCallee->mEmittedValue) { // we lazy emit call targets to save constpool space
            if (auto* function = dynamic_cast<Function*>(instruction.mCallee)) {
                function->mEmittedValue = bibbleasm::ConstPoolIndex(mBuilder.constPool().addFunctionInfo(getModuleInfoConstant(*mModuleName), getStringConstant(function->mName)));
            } else {
                assert(false); //TODO: revisit when extern functions
            }
        }

        int index = 0;
        for (Value* parameter : instruction.mParameters) {
            bytecode::Move(*mInstBuilder, instruction.mVRegRange[index++]->toOperand(), *parameter->mEmittedValue);
        }
        bytecode::Call(*mInstBuilder, instruction.mVReg->toOperand(), *instruction.mCallee->mEmittedValue, instruction.mParameters.empty() ? bibbleasm::Register(0) : instruction.mVRegRange.front()->toOperand());

        instruction.mEmittedValue = instruction.mVReg->toOperand();
    }

    void CodegenVisitor::visit(PhiInstruction& instruction) {
        std::vector<BasicBlock*> done;
        for (auto& incoming : instruction.mIncoming) {
            if (std::ranges::find(done, incoming.second) != done.end()) continue;
            if (incoming.second->exists()) {
                if (incoming.first == &instruction) continue;

                done.push_back(incoming.second);
                std::optional<bibbleasm::Instruction> move = bytecode::BuildMove(*instruction.mEmittedValue, *incoming.first->mEmittedValue);
                if (move) {
                    incoming.second->endId() = mInstBuilder->assembler().emit(incoming.second->endId(), *move);
                }
            }
        }
    }

    void CodegenVisitor::visit(ReturnInstruction& instruction) {
        instruction.mParent->endId() = mInstBuilder->assembler().getLastInstructionId();

        if (!instruction.mReturnValue) {
            mInstBuilder->load_imm(bibbleasm::Register(0), bibbleasm::Immediate(67));
            mInstBuilder->return_(bibbleasm::Register(0)); // if the regalloc always makes sure there's 1 register available on void functions, we can do this safely
        } else {
            if (std::holds_alternative<bibbleasm::Register>(*instruction.mReturnValue->mEmittedValue)) { // in this case, the return value uses a register and that can be directly returned
                mInstBuilder->return_(std::get<bibbleasm::Register>(*instruction.mReturnValue->mEmittedValue));
            } else {
                bytecode::Move(*mInstBuilder, bibbleasm::Register(0), instruction.mReturnValue->mEmittedValue.value());
                mInstBuilder->return_(bibbleasm::Register(0));
            }
        }
    }

    void CodegenVisitor::visit(UnaryInstruction& instruction) {
        bibbleasm::Register operandReg = std::get<bibbleasm::Register>(*instruction.mOperand->mEmittedValue);
        bibbleasm::Register dst = instruction.mVReg->toOperand();

        switch (instruction.getOperator()) {
            case UnaryInstruction::NEG:
                mInstBuilder->neg(dst, operandReg);
                break;
            case UnaryInstruction::ABS:
                mInstBuilder->abs(dst, operandReg);
                break;
            case UnaryInstruction::NOT:
                mInstBuilder->not_(dst, operandReg);
                break;
        }

        instruction.mEmittedValue = dst;
    }

    bibbleasm::ConstantIndex CodegenVisitor::getStringConstant(const std::string& str) {
        auto it = mStringConstants.find(str);
        if (it == mStringConstants.end()) {
            bibbleasm::ConstantIndex idx = mBuilder.constPool().addString(str);
            mStringConstants[str] = idx;
            return idx;
        }
        return it->second;
    }

    bibbleasm::ConstantIndex CodegenVisitor::getModuleInfoConstant(const std::string& name) {
        auto it = mModuleInfoConstants.find(name);
        if (it == mModuleInfoConstants.end()) {
            bibbleasm::ConstantIndex idx = mBuilder.constPool().addModuleInfo(getStringConstant(name));
            mModuleInfoConstants[name] = idx;
            return idx;
        }
        return it->second;
    }
}
