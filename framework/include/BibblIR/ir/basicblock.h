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

        void insertValue(Value* value);
        void insertValue(Value* insertAfter, Value* value);
        void eraseValue(Value* value);

        void accept(Visitor& visitor) override;

    private:
        std::string mName;
        Function* mParent;

        std::vector<ValuePtr> mValueList;

        BasicBlock(std::string name, Function* parent);

        void setEmittedValue(); // called when emitting a function
    };
}

#endif //BIBBLIR_IR_BASICBLOCK_H
