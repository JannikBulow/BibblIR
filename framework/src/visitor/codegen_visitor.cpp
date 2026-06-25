// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/bytecode/utils.h"

#include "BibblIR/ir/instruction/binary_instruction.h"
#include "BibblIR/ir/instruction/return_instruction.h"

#include "BibblIR/ir/function.h"

#include "BibblIR/optimizer/regalloc/allocator.h"

#include "BibblIR/visitor/codegen_visitor.h"

#include "BibblIR/module.h"
#include "BibblIR/ir/constant/constant_boolean.h"
#include "BibblIR/ir/constant/constant_int.h"

namespace bibblir {
    bibbleasm::Module CodegenVisitor::buildModule() {
        bibbleasm::Module module = mBuilder.build();
        mBuilder = {}; // reset
        return module;
    }

    void CodegenVisitor::visit(Module& module) {
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

        mInstBuilder = nullptr;
    }

    void CodegenVisitor::visit(BasicBlock& block) {
        if (block.exists()) {
            mInstBuilder->label(block.mName);
            for (const ValuePtr& value : block.mValueList) {
                value->accept(*this);
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

        if (instruction.getType()->isIntegerType()) {
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

    void CodegenVisitor::visit(ReturnInstruction& instruction) {
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

    bibbleasm::ConstantIndex CodegenVisitor::getStringConstant(const std::string& str) {
        auto it = mStringConstants.find(str);
        if (it == mStringConstants.end()) {
            bibbleasm::ConstantIndex idx = mBuilder.constPool().addString(str);
            mStringConstants[str] = idx;
            return idx;
        }
        return it->second;
    }
}
