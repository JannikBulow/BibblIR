// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_BASICBLOCK_H
#define BIBBLIR_IR_BASICBLOCK_H

#include "BibblIR/ir/value.h"

#include <string>
#include <vector>

namespace bibblir {
    class Function;

    class BIBBLIR_EXPORT BasicBlock : public Value {
        friend class Function;
        friend class PrintVisitor;
    public:
        Function* getParent() const;

        void insertValue(ValuePtr value);
        void insertValue(Value* insertAfter, ValuePtr value);
        void eraseValue(Value* value);

        std::vector<Value*>& liveIn();
        std::vector<BasicBlock*>& successors();
        std::vector<BasicBlock*>& predecessors();
        BasicBlock*& loopEnd();
        bool exists() const;

        std::string identifier() const override;

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
    };

    using BasicBlockPtr = std::unique_ptr<BasicBlock>;
}

#endif //BIBBLIR_IR_BASICBLOCK_H
