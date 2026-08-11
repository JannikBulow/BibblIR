// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/bytecode/utils.h"

#include "BibblIR/ir/constant/constant_boolean.h"
#include "BibblIR/ir/constant/constant_int.h"

#include "BibblIR/ir/instruction/binary_instruction.h"
#include "BibblIR/ir/instruction/branch_instruction.h"
#include "BibblIR/ir/instruction/call_instruction.h"
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

#include "BibblIR/type/class_type.h"

#include "BibblIR/visitor/codegen_visitor.h"

#include "BibblIR/module.h"

#include <algorithm>
#include <cassert>

namespace bibblir {
    bibbleasm::ModuleBuilder CodegenVisitor::stealBuilder() {
        //TODO: reset the whole visitor, i'm too lazy to rn
        return std::move(mBuilder);
    }

    void CodegenVisitor::printDisassembly(std::ostream& stream) {
        const auto& constPool = mBuilder.constPool();
        for (uint16_t i = 1; i < constPool.getEntryCount(); i++) {
            stream << constPool.stringifyEntry(i) << "\n";
        }
        if (constPool.getEntryCount() > 1) stream << '\n';

        const auto& classes = mBuilder.classes();
        for (const auto& clas : classes) {
            stream << "class " << clas->getName() << ":\n";
            stream << "field printing not yet added\nmethod printing not yet added\n\n";
        }

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
            global->accept(*this);
        }

        mModuleName = nullptr;
    }

    void CodegenVisitor::visit(Class& clas) {
        bibbleasm::ClassBuilder& classBuilder = mBuilder.addClass(getStringConstant(clas.mName));
        mClassBuilder = &classBuilder;

        for (const auto& field : clas.mFields) {
            field->accept(*this);
        }

        for (const auto& method : clas.mMethods) {
            method->accept(*this);
        }

        mClassBuilder = nullptr;
    }

    void CodegenVisitor::visit(Field& field) {
        mClassBuilder->addField(field.getType()->getIDByte(), getStringConstant(field.mName));
    }

    void CodegenVisitor::visit(Method& method) {
        if (method.mImpl != nullptr) {
            const std::string* implModuleName = nullptr;
            const std::string* implFunctionName = nullptr;

            if (auto* function = dynamic_cast<Function*>(method.mImpl)) {
                implModuleName = mModuleName;
                implFunctionName = &function->mName;
            } else if (auto* function = dynamic_cast<ExternalFunction*>(method.mImpl)) {
                implModuleName = &function->mModuleName;
                implFunctionName = &function->mName;
            } else {
                assert(false); // do NOT implement a method with a constant int...
            }

            mClassBuilder->addMethod(getStringConstant(method.mName), getFunctionInfoConstant(*implModuleName, *implFunctionName));
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

        for (const BasicBlockPtr& bb : function.basicBlocks()) {
            for (PhiInstruction* phi : bb->mPhis) {
                phi->accept(*this);
            }

            for (auto& [predecessor, moves] : bb->mPhiCopies) {
                emitParallelMoves(predecessor, moves);
            }
        }

        mInstBuilder = nullptr;
    }

    void CodegenVisitor::visit(ExternalFunction& function) {

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
        arg.mEmittedValue = arg.mVReg->toOperand();
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
                function->mEmittedValue = bibbleasm::ConstPoolIndex(getFunctionInfoConstant(*mModuleName, function->mName));
            } else if (auto* function = dynamic_cast<ExternalFunction*>(instruction.mCallee)) {
                function->mEmittedValue = bibbleasm::ConstPoolIndex(getFunctionInfoConstant(function->mModuleName, function->mName));
            } else {
                assert(false);
            }
        }

        int index = 0;
        for (Value* parameter : instruction.mParameters) {
            bytecode::Move(*mInstBuilder, instruction.mVRegRange[index++]->toOperand(), *parameter->mEmittedValue);
        }
        bytecode::Call(*mInstBuilder, instruction.mVReg->toOperand(), *instruction.mCallee->mEmittedValue, instruction.mParameters.empty() ? bibbleasm::Register(0) : instruction.mVRegRange.front()->toOperand());

        instruction.mEmittedValue = instruction.mVReg->toOperand();
    }

    void CodegenVisitor::visit(GetMemberInstruction& instruction) {
        // everything about this instruction is super shit. it's handled in load/store
    }

    void CodegenVisitor::visit(IntCastInstruction& instruction) {
        auto vreg = instruction.mVReg->toOperand();
        bytecode::Move(*mInstBuilder, vreg, *instruction.mValue->mEmittedValue);
        instruction.mEmittedValue = vreg;
    }

    void CodegenVisitor::visit(LoadInstruction& instruction) {
        auto vreg = instruction.mVReg->toOperand();

        if (auto* getmember = dynamic_cast<GetMemberInstruction*>(instruction.mVariable)) {
            if (auto* field = dynamic_cast<Field*>(getmember->mMember)) {
                if (auto* classType = dynamic_cast<ClassType*>(getmember->mObject->getType())) {
                    mInstBuilder->getfield(vreg, std::get<bibbleasm::Register>(*getmember->mObject->mEmittedValue), getFieldInfoConstant(std::string(classType->getModuleName()), std::string(classType->getClassName()), field->mName));
                    // WHAT THE FUCK IS THIS
                }
            } else if (auto* method = dynamic_cast<Method*>(getmember->mMember)) {
                if (auto* classType = dynamic_cast<ClassType*>(getmember->mObject->getType())) {
                    mInstBuilder->dispatchmethod(vreg, std::get<bibbleasm::Register>(*getmember->mObject->mEmittedValue), getMethodInfoConstant(std::string(classType->getModuleName()), std::string(classType->getClassName()), method->mName));
                }
            }
        } else {
            bytecode::Move(*mInstBuilder, vreg, *instruction.mVariable->mEmittedValue);
        }

        instruction.mEmittedValue = vreg;
    }

    void CodegenVisitor::visit(NewInstruction& instruction) {
        auto vreg = instruction.mVReg->toOperand();

        if (auto* clas = dynamic_cast<ClassType*>(instruction.getType())) {
            mInstBuilder->newinstance(vreg, getClassInfoConstant(std::string(clas->getModuleName()), std::string(clas->getClassName())));
        }

        instruction.mEmittedValue = vreg;
    }

    void CodegenVisitor::visit(PhiInstruction& instruction) {
        for (auto& [value, predecessor] : instruction.mIncoming) {
            if (!predecessor->exists()) continue;
            if (value == &instruction) continue;

            instruction.mParent->mPhiCopies[predecessor].emplace_back(*instruction.mEmittedValue, *value->mEmittedValue);
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

    void CodegenVisitor::visit(StoreInstruction& instruction) {
        if (auto* fieldInstruction = dynamic_cast<GetMemberInstruction*>(instruction.mVariable)) {
            if (auto* field = dynamic_cast<Field*>(fieldInstruction->mMember)) {
                if (auto* classType = dynamic_cast<ClassType*>(fieldInstruction->mObject->getType())) {
                    mInstBuilder->setfield(std::get<bibbleasm::Register>(*fieldInstruction->mObject->mEmittedValue), getFieldInfoConstant(std::string(classType->getModuleName()), std::string(classType->getClassName()), field->mName), std::get<bibbleasm::Register>(*instruction.mValue->mEmittedValue));
                    // WHAT THE FUCK IS THIS
                }
            }
        } else {
            bytecode::Move(*mInstBuilder, *instruction.mVariable->mEmittedValue, *instruction.mValue->mEmittedValue);
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

    void CodegenVisitor::emitParallelMoves(BasicBlock* bb, std::vector<BasicBlock::ParallelMove> moves) {
        auto emitMove = [this](BasicBlock* bb, bibbleasm::Operand dst, bibbleasm::Operand src) {
            auto move = bytecode::BuildMove(dst, src);
            if (!move) return;

            bb->endId() = mInstBuilder->assembler().emit(bb->endId(), *move);
        };

        std::erase_if(moves, [](const auto& move) {
            return move.dst == move.src;
        });

        while (!moves.empty()) {
            bool madeProgress = false;

            for (size_t i = 0; i < moves.size(); i++) {
                const auto& move = moves[i];

                bool sourceIsNeeded = std::ranges::any_of(moves, [&move](const auto& other) {
                    return other.dst != move.dst && other.src == move.dst;
                });

                if (!sourceIsNeeded) {
                    emitMove(bb, move.dst, move.src);
                    moves.erase(moves.begin() + i);
                    madeProgress = true;
                    break;
                }
            }

            if (madeProgress) continue;

            auto& move = moves.front();

            VReg* scratch = bb->getParent()->getScratchVReg();

            emitMove(bb, scratch->toOperand(), move.dst);

            for (auto& pending : moves) {
                if (pending.src == move.dst) {
                    pending.src = scratch->toOperand();
                }
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

    bibbleasm::ConstantIndex CodegenVisitor::getModuleInfoConstant(const std::string& name) {
        auto it = mModuleInfoConstants.find(name);
        if (it == mModuleInfoConstants.end()) {
            bibbleasm::ConstantIndex idx = mBuilder.constPool().addModuleInfo(getStringConstant(name));
            mModuleInfoConstants[name] = idx;
            return idx;
        }
        return it->second;
    }

    bibbleasm::ConstantIndex CodegenVisitor::getFunctionInfoConstant(const std::string& moduleName, const std::string& name) {
        TwoString strings(moduleName, name);

        auto it = mFunctionInfoConstants.find(strings);
        if (it == mFunctionInfoConstants.end()) {
            bibbleasm::ConstantIndex idx = mBuilder.constPool().addFunctionInfo(getModuleInfoConstant(moduleName), getStringConstant(name));
            mFunctionInfoConstants[std::move(strings)] = idx;
            return idx;
        }
        return it->second;
    }

    bibbleasm::ConstantIndex CodegenVisitor::getClassInfoConstant(const std::string& moduleName, const std::string& name) {
        TwoString strings(moduleName, name);

        auto it = mClassInfoConstants.find(strings);
        if (it == mClassInfoConstants.end()) {
            bibbleasm::ConstantIndex idx = mBuilder.constPool().addClassInfo(getModuleInfoConstant(moduleName), getStringConstant(name));
            mClassInfoConstants[std::move(strings)] = idx;
            return idx;
        }
        return it->second;
    }

    bibbleasm::ConstantIndex CodegenVisitor::getFieldInfoConstant(const std::string& moduleName, const std::string& className, const std::string& name) {
        ThreeString strings(moduleName, className, name);

        auto it = mFieldInfoConstants.find(strings);
        if (it == mFieldInfoConstants.end()) {
            bibbleasm::ConstantIndex idx = mBuilder.constPool().addFieldInfo(getClassInfoConstant(moduleName, className), getStringConstant(name));
            mFieldInfoConstants[std::move(strings)] = idx;
            return idx;
        }
        return it->second;
    }

    bibbleasm::ConstantIndex CodegenVisitor::getMethodInfoConstant(const std::string& moduleName, const std::string& className, const std::string& name) {
        ThreeString strings(moduleName, className, name);

        auto it = mMethodInfoConstants.find(strings);
        if (it == mMethodInfoConstants.end()) {
            bibbleasm::ConstantIndex idx = mBuilder.constPool().addMethodInfo(getClassInfoConstant(moduleName, className), getStringConstant(name));
            mMethodInfoConstants[std::move(strings)] = idx;
            return idx;
        }
        return it->second;
    }
}
