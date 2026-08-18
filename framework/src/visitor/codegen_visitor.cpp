// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/bytecode/utils.h"

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

#include "BibblIR/type/array_type.h"
#include "BibblIR/type/class_type.h"

#include "BibblIR/visitor/codegen_visitor.h"

#include "BibblIR/module.h"

#include <algorithm>
#include <cassert>

namespace bibblir {
    using namespace bibbleasm;

    template<class T>
    static bool IsAlternative(const std::optional<Operand>& operand) {
        if (!operand.has_value()) return false;
        return std::holds_alternative<T>(*operand);
    }

    template<class T>
    static const T& GetAlternative(const std::optional<Operand>& operand) {
        assert(operand.has_value());
        return std::get<T>(*operand);
    }

    ModuleBuilder CodegenVisitor::stealBuilder() {
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

        for (const ValuePtr& constant : module.getConstants()) {
            constant->accept(*this);
        }

        for (const GlobalPtr& global : module.getGlobals()) {
            global->accept(*this);
        }

        mModuleName = nullptr;
    }

    void CodegenVisitor::visit(Class& clas) {
        ClassBuilder& classBuilder = mBuilder.addClass(getStringConstant(clas.mName));
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

        FunctionBuilder& functionBuilder = mBuilder.addFunction(getStringConstant(function.mName), flags, function.mRegisterCount, function.mArguments.size());
        InstructionBuilder instructionBuilder(functionBuilder.assembler());
        mInstBuilder = &instructionBuilder;

        for (const BasicBlockPtr& bb : function.basicBlocks()) {
            bb->mEmittedValue = Label(bb->mName);
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
        constant.mEmittedValue = Immediate(OperandSize::Byte, constant.mValue);
    }

    void CodegenVisitor::visit(ConstantInt& constant) {
        constant.mEmittedValue = Immediate(constant.mValue);
    }

    void CodegenVisitor::visit(BinaryInstruction& instruction) {
        if (instruction.isConstantFolded()) {
            instruction.mEmittedValue = Immediate(static_cast<int64_t>(instruction.getConstantFoldedValue()));
            return;
        }

        ScratchManager scratchManager(instruction);

        Register leftReg(-1);
        Register rightReg(-1);

        if (IsAlternative<Register>(instruction.getLeft()->getEmittedValue())) {
            leftReg = GetAlternative<Register>(instruction.getLeft()->getEmittedValue());
        } else {
            leftReg = scratchManager.get()->toOperand();
            bytecode::Move(*mInstBuilder, leftReg, *instruction.getLeft()->getEmittedValue());
        }

        if (IsAlternative<Register>(instruction.getRight()->getEmittedValue())) {
            rightReg = GetAlternative<Register>(instruction.getRight()->getEmittedValue());
        } else {
            rightReg = scratchManager.get()->toOperand();
            bytecode::Move(*mInstBuilder, rightReg, *instruction.getRight()->getEmittedValue());
        }

        Register dst = instruction.mVReg->toOperand();

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
        } else if (instruction.mLeft->getType()->isFloatType()) {
            switch (instruction.mOperator) {
                case BinaryInstruction::ADD:
                    mInstBuilder->fadd(dst, leftReg, rightReg);
                    break;
                case BinaryInstruction::SUB:
                    mInstBuilder->fsub(dst, leftReg, rightReg);
                    break;
                case BinaryInstruction::MUL:
                    mInstBuilder->mul(dst, leftReg, rightReg);
                    break;
                case BinaryInstruction::SDIV:
                    mInstBuilder->fdiv(dst, leftReg, rightReg);
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
                case BinaryInstruction::NE:
                case BinaryInstruction::LT:
                case BinaryInstruction::GT:
                case BinaryInstruction::LE:
                case BinaryInstruction::GE:
                    mInstBuilder->fcmp(dst, leftReg, rightReg);
                    break;
            }
        }

        instruction.mEmittedValue = dst; // the conditional branch codegen could simply check if its condition is a BinaryInstruction, then check the operator
    }

    void CodegenVisitor::visit(BranchInstruction& instruction) {
        assert(instruction.getParent()->endId() == static_cast<InstructionId>(-1));
        instruction.getParent()->endId() = mInstBuilder->assembler().getLastInstructionId();

        if (instruction.isConstantFolded() && !instruction.trueBranch()) {
            bytecode::Jump(*mInstBuilder, *instruction.falseBranch()->getEmittedValue());
        }

        if (!instruction.falseBranch()) {
            bytecode::Jump(*mInstBuilder, *instruction.trueBranch()->getEmittedValue());
        } else {
            if (auto binary = dynamic_cast<BinaryInstruction*>(instruction.getCondition())) {
                bytecode::CondType condType;
                switch (binary->getOperator()) {
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

                bytecode::CondJump(*mInstBuilder, condType, *instruction.getCondition()->getEmittedValue(),  *instruction.trueBranch()->getEmittedValue());
                bytecode::Jump(*mInstBuilder, *instruction.falseBranch()->getEmittedValue());
            } else if (IsAlternative<Immediate>(instruction.getCondition()->getEmittedValue())) {
                Immediate imm = GetAlternative<Immediate>(instruction.getCondition()->getEmittedValue());
                if (imm.value) {
                    bytecode::Jump(*mInstBuilder, *instruction.trueBranch()->getEmittedValue());
                } else {
                    bytecode::Jump(*mInstBuilder, *instruction.falseBranch()->getEmittedValue());
                }
            }
        }
    }

    void CodegenVisitor::visit(CallInstruction& instruction) {
        if (!instruction.getCallee()->getEmittedValue()) { // we lazy emit call targets to save constpool space
            if (auto* function = dynamic_cast<Function*>(instruction.getCallee())) {
                function->mEmittedValue = ConstPoolIndex(getFunctionInfoConstant(*mModuleName, function->mName));
            } else if (auto* function = dynamic_cast<ExternalFunction*>(instruction.getCallee())) {
                function->mEmittedValue = ConstPoolIndex(getFunctionInfoConstant(function->mModuleName, function->mName));
            } else {
                assert(false);
            }
        }

        int index = 0;
        for (Value* parameter : instruction.getParameters()) {
            bytecode::Move(*mInstBuilder, instruction.mVRegRange[index++]->toOperand(), *parameter->getEmittedValue());
        }

        Register reg = instruction.mVReg->toOperand();

        bytecode::Call(*mInstBuilder, reg, *instruction.getCallee()->getEmittedValue(), instruction.getParameters().empty() ? Register(0) : instruction.mVRegRange.front()->toOperand());

        instruction.mEmittedValue = reg;
    }

    void CodegenVisitor::visit(GetElementInstruction& instruction) {
        // everything about this instruction is super shit. it's handled in load/store
    }

    void CodegenVisitor::visit(GetMemberInstruction& instruction) {
        // everything about this instruction is super shit. it's handled in load/store
    }

    void CodegenVisitor::visit(IntCastInstruction& instruction) {
        Register reg = instruction.mVReg->toOperand();
        bytecode::Move(*mInstBuilder, reg, *instruction.getValue()->getEmittedValue());
        instruction.mEmittedValue = reg;
    }

    void CodegenVisitor::visit(LoadInstruction& instruction) {
        Register reg = instruction.mVReg->toOperand();

        if (auto* getmember = dynamic_cast<GetMemberInstruction*>(instruction.getVariable())) {
            if (auto* field = dynamic_cast<Field*>(getmember->getMember())) {
                if (auto* classType = dynamic_cast<ClassType*>(getmember->getObject()->getType())) {
                    mInstBuilder->getfield(reg, GetAlternative<Register>(getmember->getObject()->getEmittedValue()), getFieldInfoConstant(std::string(classType->getModuleName()), std::string(classType->getClassName()), field->mName));
                    // WHAT THE FUCK IS THIS
                }
            } else if (auto* method = dynamic_cast<Method*>(getmember->getMember())) {
                if (auto* classType = dynamic_cast<ClassType*>(getmember->getObject()->getType())) {
                    mInstBuilder->dispatchmethod(reg, GetAlternative<Register>(getmember->getObject()->getEmittedValue()), getMethodInfoConstant(std::string(classType->getModuleName()), std::string(classType->getClassName()), method->mName));
                }
            }
        } else if (auto* getelement = dynamic_cast<GetElementInstruction*>(instruction.getVariable())) {
            mInstBuilder->arrayget(reg, GetAlternative<Register>(getelement->getArray()->getEmittedValue()), GetAlternative<Register>(getelement->getIndex()->getEmittedValue()));
        } else {
            bytecode::Move(*mInstBuilder, reg, *instruction.getVariable()->getEmittedValue());
        }

        instruction.mEmittedValue = reg;
    }

    void CodegenVisitor::visit(NewInstruction& instruction) {
        Register reg = instruction.mVReg->toOperand();

        if (auto* clas = dynamic_cast<ClassType*>(instruction.getType())) {
            mInstBuilder->newinstance(reg, getClassInfoConstant(std::string(clas->getModuleName()), std::string(clas->getClassName())));
        } else if (auto* array = dynamic_cast<ArrayType*>(instruction.getType())) {
            mInstBuilder->newarray(reg, std::get<Register>(*instruction.getParameter()->getEmittedValue()), array->getElementType()->getIDByte());
        }

        instruction.mEmittedValue = reg;
    }

    void CodegenVisitor::visit(PhiInstruction& instruction) {
        for (auto& [value, predecessor] : instruction.incoming()) {
            if (!predecessor->exists()) continue;
            if (value == &instruction) continue;

            instruction.getParent()->mPhiCopies[predecessor].emplace_back(*instruction.getEmittedValue(), *value->getEmittedValue());
        }
    }

    void CodegenVisitor::visit(ReturnInstruction& instruction) {
        instruction.getParent()->endId() = mInstBuilder->assembler().getLastInstructionId();

        if (!instruction.getReturnValue()) {
            //mInstBuilder->load_imm(Register(0), Immediate(67));
            mInstBuilder->return_(Register(0)); // if the regalloc always makes sure there's 1 register available on void functions, we can do this safely
        } else {
            if (IsAlternative<Register>(*instruction.getReturnValue()->getEmittedValue())) { // in this case, the return value uses a register and that can be directly returned
                mInstBuilder->return_(std::get<Register>(*instruction.getReturnValue()->getEmittedValue()));
            } else {
                bytecode::Move(*mInstBuilder, Register(0), *instruction.getReturnValue()->getEmittedValue());
                mInstBuilder->return_(Register(0));
            }
        }
    }

    void CodegenVisitor::visit(StoreInstruction& instruction) {
        ScratchManager scratchManager(instruction);
        Register valueReg(0xFFFF);

        if (IsAlternative<Register>(instruction.getValue()->getEmittedValue())) {
            valueReg = GetAlternative<Register>(instruction.getValue()->getEmittedValue());
        } else {
            valueReg = scratchManager.get()->toOperand();
            bytecode::Move(*mInstBuilder, valueReg, *instruction.getValue()->getEmittedValue());
        }

        if (auto* getmember = dynamic_cast<GetMemberInstruction*>(instruction.getVariable())) {
            if (auto* field = dynamic_cast<Field*>(getmember->getMember())) {
                if (auto* classType = dynamic_cast<ClassType*>(getmember->getObject()->getType())) {
                    mInstBuilder->setfield(GetAlternative<Register>(getmember->getObject()->getEmittedValue()), getFieldInfoConstant(std::string(classType->getModuleName()), std::string(classType->getClassName()), field->mName), valueReg);
                    // WHAT THE FUCK IS THIS
                }
            }
        } else if (auto* getelement = dynamic_cast<GetElementInstruction*>(instruction.mVariable)) {
            Register indexReg(0xFFFF);

            if (IsAlternative<Register>(getelement->getIndex()->getEmittedValue())) {
                indexReg = GetAlternative<Register>(getelement->getIndex()->getEmittedValue());
            } else {
                indexReg = scratchManager.get()->toOperand();
                bytecode::Move(*mInstBuilder, indexReg, *getelement->getIndex()->getEmittedValue());
            }

            mInstBuilder->arrayset(GetAlternative<Register>(getelement->getArray()->getEmittedValue()), indexReg, valueReg);
        } else {
            bytecode::Move(*mInstBuilder, *instruction.getVariable()->getEmittedValue(), *instruction.getValue()->getEmittedValue());
        }
    }

    void CodegenVisitor::visit(UnaryInstruction& instruction) {
        if (instruction.isConstantFolded()) {
            instruction.mEmittedValue = Immediate(static_cast<int64_t>(instruction.getConstantFoldedValue()));
            return;
        }

        ScratchManager scratchManager(instruction);
        Register operandReg(0xFFFF);

        if (IsAlternative<Register>(instruction.getOperand()->getEmittedValue())) {
            operandReg = GetAlternative<Register>(instruction.getOperand()->getEmittedValue());
        } else {
            operandReg = scratchManager.get()->toOperand();
            bytecode::Move(*mInstBuilder, operandReg, *instruction.getOperand()->getEmittedValue());
        }

        Register dst = instruction.mVReg->toOperand();

        if (instruction.mOperand->getType()->isIntegerType()) {
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
        } else if (instruction.mOperand->getType()->isFloatType()) {
            switch (instruction.getOperator()) {
                case UnaryInstruction::NEG:
                    mInstBuilder->fneg(dst, operandReg);
                    break;
                case UnaryInstruction::ABS:
                    mInstBuilder->fabs(dst, operandReg);
                    break;
                case UnaryInstruction::NOT:
                    assert(false);
                    break;
            }
        }

        instruction.mEmittedValue = dst;
    }

    CodegenVisitor::ScratchManager::~ScratchManager() {
        value.getParent()->getParent()->resetScratches();
    }

    VReg* CodegenVisitor::ScratchManager::get() {
        if (!valueVRegUsed && value.mVReg) {
            valueVRegUsed = true;
            return value.mVReg;
        }

        return value.getParent()->getParent()->getScratchVReg();
    }

    void CodegenVisitor::emitParallelMoves(BasicBlock* bb, std::vector<BasicBlock::ParallelMove> moves) {
        auto emitMove = [this](BasicBlock* bb, Operand dst, Operand src) {
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

    ConstantIndex CodegenVisitor::getStringConstant(const std::string& str) {
        auto it = mStringConstants.find(str);
        if (it == mStringConstants.end()) {
            ConstantIndex idx = mBuilder.constPool().addString(str);
            mStringConstants[str] = idx;
            return idx;
        }
        return it->second;
    }

    ConstantIndex CodegenVisitor::getModuleInfoConstant(const std::string& name) {
        auto it = mModuleInfoConstants.find(name);
        if (it == mModuleInfoConstants.end()) {
            ConstantIndex idx = mBuilder.constPool().addModuleInfo(getStringConstant(name));
            mModuleInfoConstants[name] = idx;
            return idx;
        }
        return it->second;
    }

    ConstantIndex CodegenVisitor::getFunctionInfoConstant(const std::string& moduleName, const std::string& name) {
        TwoString strings(moduleName, name);

        auto it = mFunctionInfoConstants.find(strings);
        if (it == mFunctionInfoConstants.end()) {
            ConstantIndex idx = mBuilder.constPool().addFunctionInfo(getModuleInfoConstant(moduleName), getStringConstant(name));
            mFunctionInfoConstants[std::move(strings)] = idx;
            return idx;
        }
        return it->second;
    }

    ConstantIndex CodegenVisitor::getClassInfoConstant(const std::string& moduleName, const std::string& name) {
        TwoString strings(moduleName, name);

        auto it = mClassInfoConstants.find(strings);
        if (it == mClassInfoConstants.end()) {
            ConstantIndex idx = mBuilder.constPool().addClassInfo(getModuleInfoConstant(moduleName), getStringConstant(name));
            mClassInfoConstants[std::move(strings)] = idx;
            return idx;
        }
        return it->second;
    }

    ConstantIndex CodegenVisitor::getFieldInfoConstant(const std::string& moduleName, const std::string& className, const std::string& name) {
        ThreeString strings(moduleName, className, name);

        auto it = mFieldInfoConstants.find(strings);
        if (it == mFieldInfoConstants.end()) {
            ConstantIndex idx = mBuilder.constPool().addFieldInfo(getClassInfoConstant(moduleName, className), getStringConstant(name));
            mFieldInfoConstants[std::move(strings)] = idx;
            return idx;
        }
        return it->second;
    }

    ConstantIndex CodegenVisitor::getMethodInfoConstant(const std::string& moduleName, const std::string& className, const std::string& name) {
        ThreeString strings(moduleName, className, name);

        auto it = mMethodInfoConstants.find(strings);
        if (it == mMethodInfoConstants.end()) {
            ConstantIndex idx = mBuilder.constPool().addMethodInfo(getClassInfoConstant(moduleName, className), getStringConstant(name));
            mMethodInfoConstants[std::move(strings)] = idx;
            return idx;
        }
        return it->second;
    }
}
