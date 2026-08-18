// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_FUNCTION_H
#define BIBBLIR_IR_FUNCTION_H

#include "BibblIR/ir/abstract_function.h"
#include "BibblIR/ir/basicblock.h"

#include "BibblIR/type/function_type.h"

namespace bibblir {
    class BIBBLIR_EXPORT Argument : public Value {
        friend class CodegenVisitor;
        friend class PrintVisitor;
        friend class RegAlloc;
    public:
        Argument(Module& module, Type* type, int index);

        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        int mValueId;
        int mIndex;
    };

    using ArgumentPtr = std::unique_ptr<Argument>;

    class BIBBLIR_EXPORT Function : public AbstractFunction {
        friend class CodegenVisitor;
        friend class PrintVisitor;
        friend class RegAlloc;
    public:
        static Function* Create(Module& module, FunctionType* type, std::string name);

        FunctionType* getFunctionType() const;
        Argument* getArgument(int index) const;
        const std::vector<ArgumentPtr>& getArguments() const;

        BasicBlock* createBasicBlock(std::string name);
        size_t getBasicBlockCount() const;
        std::vector<BasicBlockPtr>& basicBlocks();

        void replaceAllUsesWith(Value* oldValue, Value* newValue);

        std::string_view getModuleName() const override;
        std::string_view getName() const override;

        std::string identifier() const override;

        void accept(Visitor& visitor) override;

    private:
        struct Scratch {
            VReg* vreg;
            bool inUse;
        };

        std::string mName;
        std::vector<ArgumentPtr> mArguments;

        std::vector<BasicBlockPtr> mBasicBlockList;

        std::vector<std::unique_ptr<VReg>> mVRegs;
        std::vector<Scratch> mScratches;
        uint16_t mRegisterCount = 1; // real registers

        Function(Module& module, FunctionType* type, std::string name);

        VReg* getScratchVReg();
        void resetScratches();
    };
}

#endif //BIBBLIR_IR_FUNCTION_H
