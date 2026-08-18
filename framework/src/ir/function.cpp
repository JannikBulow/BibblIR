// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/function.h"

#include "BibblIR/module.h"
#include "BibblIR/visitor/visitor.h"

#include <algorithm>
#include <stack>

namespace bibblir {
    Argument::Argument(Module& module, Type* type, int index)
        : Value(module)
        , mValueId(mModule.getNextValueId())
        , mIndex(index) {
        mType = type;
    }

    std::string Argument::identifier() const {
        return getName(mValueId);
    }

    void Argument::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    Function* Function::Create(Module& module, FunctionType* type, std::string name) {
        Function* func = new Function(module, type, std::move(name));
        module.insertGlobal(GlobalPtr(func));
        return func;
    }

    FunctionType* Function::getFunctionType() const {
        return static_cast<FunctionType*>(mType);
    }

    Argument* Function::getArgument(int index) const {
        if (index < mArguments.size()) return mArguments[index].get();
        return nullptr;
    }

    const std::vector<ArgumentPtr>& Function::getArguments() const {
        return mArguments;
    }

    BasicBlock* Function::createBasicBlock(std::string name) {
        if (name.empty()) {
            name = std::to_string(mModule.getNextValueId());
        }

        BasicBlock* bb = new BasicBlock(std::move(name), this);
        mBasicBlockList.push_back(BasicBlockPtr(bb));
        return bb;
    }

    size_t Function::getBasicBlockCount() const {
        return mBasicBlockList.size();
    }

    std::vector<BasicBlockPtr>& Function::basicBlocks() {
        return mBasicBlockList;
    }

    void Function::replaceAllUsesWith(Value* oldValue, Value* newValue) {
        std::vector<Value*> done;
        for (auto& bb : mBasicBlockList) {
            for (auto& value : bb->mValueList) {
                done.push_back(value.get());
                for (auto operand : value->getOperands()) {
                    if (operand == oldValue) {
                        operand.get() = newValue;
                    }
                }
            }
        }
    }

    std::string Function::identifier() const {
        return mName;
    }

    void Function::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    Function::Function(Module& module, FunctionType* type, std::string name)
        : Global(module)
        , mName(std::move(name)) {
        mType = type;

        int index = 0;
        for (Type* type : getFunctionType()->getArgumentTypes()) {
            mArguments.push_back(std::make_unique<Argument>(module, type, index++));
        }
    }

    VReg* Function::getScratchVReg() {
        for (auto& scratch : mScratches) {
            if (!scratch.inUse) {
                scratch.inUse = true;
                scratch.vreg->mUses++;
                return scratch.vreg;
            }
        }

        uint16_t index = mRegisterCount++;
        mVRegs.push_back(std::make_unique<VReg>(index, index));
        mScratches.emplace_back(mVRegs.back().get(), true);
        mScratches.back().vreg->mUses++;
        return mScratches.back().vreg;
    }

    void Function::resetScratches() {
        for (auto& scratch : mScratches) {
            scratch.inUse = false;
        }
    }
}
