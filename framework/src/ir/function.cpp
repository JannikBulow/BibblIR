// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/function.h"

#include "BibblIR/module.h"
#include "BibblIR/visitor/visitor.h"

#include <algorithm>
#include <stack>

namespace bibblir {
    Argument::Argument(Module& module, Type* type, std::string name, int index)
        : Value(module)
        , mName(std::move(name))
        , mIndex(index) {
        mType = type;
    }

    std::string Argument::identifier() const {
        return "%" + mName;
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
            std::string id = std::to_string(module.getNextValueId());
            mArguments.push_back(std::make_unique<Argument>(module, type, std::move(id), index++));
        }
    }

    static void Postorder(BasicBlock* head, std::vector<BasicBlock*>& visited, std::stack<BasicBlock*>& stack) {
        if (std::ranges::find(visited, head) != visited.end()) return;

        visited.push_back(head);
        for (BasicBlock* successor : head->successors()) {
            Postorder(successor, visited, stack);
        }
        stack.push(head);
    }

    void Function::orderBasicBlocks() {
        if (mBasicBlockList.empty()) return;

        std::vector<BasicBlock*> visited;
        std::stack<BasicBlock*> stack;
        Postorder(mBasicBlockList.front().get(), visited, stack);

        std::vector<BasicBlock*> blocks;
        for (auto& bb : mBasicBlockList) {
            blocks.push_back(bb.release());
        }

        mBasicBlockList.clear();

        while (!stack.empty()) {
            BasicBlock* bb = stack.top();
            stack.pop();

            mBasicBlockList.push_back(BasicBlockPtr(bb));
            std::erase(blocks, bb);
        }

        for (BasicBlock* bb : blocks) {
            bb->mExists = false;
            mBasicBlockList.push_back(BasicBlockPtr(bb));
        }
    }
}
