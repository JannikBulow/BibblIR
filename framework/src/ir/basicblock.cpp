// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction/branch_instruction.h"
#include "BibblIR/ir/instruction/return_instruction.h"

#include "BibblIR/ir/basicblock.h"
#include "BibblIR/ir/function.h"

#include "BibblIR/visitor/visitor.h"

#include <algorithm>

namespace bibblir {
    Function* BasicBlock::getParent() const {
        return mParent;
    }

    std::string_view BasicBlock::getName() const {
        return mName;
    }

    const std::vector<ValuePtr>& BasicBlock::getValues() const {
        return mValueList;
    }

    void BasicBlock::insertValue(ValuePtr value) {
        mValueList.push_back(std::move(value));
    }

    void BasicBlock::insertValue(Value* insertAfter, ValuePtr value) {
        if (insertAfter == nullptr) {
            insertValue(std::move(value));
            return;
        }

        auto it = std::ranges::find_if(mValueList, [insertAfter](const auto& value) {
            return value.get() == insertAfter;
        });
        if (it == mValueList.end()) mValueList.push_back(std::move(value));
        else mValueList.insert(it + 1, std::move(value));
    }

    void BasicBlock::eraseValue(Value* value) {
        std::erase_if(mValueList, [value](ValuePtr& valuePtr) {
            return valuePtr.get() == value;
        });
    }

    bool BasicBlock::hasTerminator() const {
        return std::any_of(mValueList.rbegin(), mValueList.rend(), [](const ValuePtr& value) {
            return dynamic_cast<BranchInstruction*>(value.get()) || dynamic_cast<ReturnInstruction*>(value.get());
        });
    }

    std::vector<Value*>& BasicBlock::liveIn() {
        return mLiveIn;
    }

    std::vector<BasicBlock*>& BasicBlock::successors() {
        return mSuccessors;
    }

    std::vector<BasicBlock*>& BasicBlock::predecessors() {
        return mPredecessors;
    }

    BasicBlock*& BasicBlock::loopEnd() {
        return mLoopEnd;
    }

    bool BasicBlock::exists() const {
        return mExists;
    }

    bibbleasm::InstructionId& BasicBlock::endId() {
        return mEndId;
    }

    void BasicBlock::disable() {
        mExists = false;
    }

    std::string BasicBlock::identifier() const {
        return "label " + mName;
    }

    void BasicBlock::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    BasicBlock::BasicBlock(std::string name, Function* parent)
        : Value(parent->getModule())
        , mName(std::move(name))
        , mParent(parent) {
        bool alldigits = true;
        for (char c : mName) {
            if (!std::isdigit(c)) {
                alldigits = false;
                break;
            }
        }

        if (alldigits) mName = "." + mName;
    }
}
