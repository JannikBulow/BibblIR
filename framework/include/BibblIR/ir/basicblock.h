// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_BASICBLOCK_H
#define BIBBLIR_IR_BASICBLOCK_H

#include "BibblIR/ir/value.h"

#include <string>
#include <vector>

namespace bibblir {
    class Function;

    class BasicBlock : public Value {
    friend class Function;
    public:
        Function* getParent() const;

        void insertValue(ValuePtr value);
        void insertValue(Value* insertAfter, ValuePtr value);
        void eraseValue(Value* value);

        std::vector<Value*>& liveIn();
        std::vector<BasicBlock*>& successors();
        std::vector<BasicBlock*>& predecessors();
        BasicBlock*& loopEnd();
        bool exists();

        void accept(Visitor& visitor) override;

    private:
        std::string mName;
        Function* mParent;

        std::vector<ValuePtr> mValueList;
        std::vector<Value*> mLiveIn;

        std::vector<BasicBlock*> mSuccessors;
        std::vector<BasicBlock*> mPredecessors;

        BasicBlock* mLoopEnd = nullptr;

        bool mExists = true;

        BasicBlock(std::string name, Function* parent);

        void setEmittedValue(); // called when emitting a function
    };

    using BasicBlockPtr = std::unique_ptr<BasicBlock>;
}

#endif //BIBBLIR_IR_BASICBLOCK_H
