// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/basicblock.h"

#include "BibblIR/ir/function.h"

#include "BibblIR/visitor/visitor.h"

#include <algorithm>

namespace bibblir {
    Function* BasicBlock::getParent() const {
        return mParent;
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

    void BasicBlock::setEmittedValue() {
        mEmittedValue = bibbleasm::Label(mName);
    }
}
