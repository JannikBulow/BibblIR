// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_FUNCTION_H
#define BIBBLIR_IR_FUNCTION_H

#include "BibblIR/ir/basicblock.h"
#include "BibblIR/ir/global.h"

#include "BibblIR/type/function_type.h"

namespace bibblir {
    class BIBBLIR_EXPORT Argument : public Value {
        friend class CodegenVisitor;
        friend class PrintVisitor;
    public:
        Argument(Module& module, Type* type, std::string name, int index);

        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        std::string mName;
        int mIndex;
    };

    using ArgumentPtr = std::unique_ptr<Argument>;

    class BIBBLIR_EXPORT Function : public Global {
        friend class CodegenVisitor;
        friend class PrintVisitor;
        friend class RegAlloc;
    public:
        static Function* Create(Module& module, FunctionType* type, std::string name);

        FunctionType* getFunctionType() const;
        Argument* getArgument(int index) const;

        BasicBlock* createBasicBlock(std::string name);
        size_t getBasicBlockCount() const;
        std::vector<BasicBlockPtr>& basicBlocks();

        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        std::string mName;
        std::vector<ArgumentPtr> mArguments;

        std::vector<BasicBlockPtr> mBasicBlockList;

        std::vector<std::unique_ptr<VReg>> mVRegs;
        uint16_t mRegisterCount = 1; // real registers

        Function(Module& module, FunctionType* type, std::string name);

        void orderBasicBlocks();
    };
}

#endif //BIBBLIR_IR_FUNCTION_H
