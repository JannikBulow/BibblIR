// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_IR_VALUE_H
#define BIBBLIR_IR_VALUE_H

#include "BibblIR/optimizer/regalloc/vreg.h"

#include "BibblIR/type/type.h"

#include "BibblIR/api.h"

#include <BibbleASM/instruction/operand.h>

#include <memory>
#include <optional>
#include <utility>
#include <vector>

namespace bibblir {
    class Module;
    class Visitor;

    class BIBBLIR_EXPORT Value {
        friend class CodegenVisitor;
    public:
        Value(Module& module)
            : mModule(module)
            , mType(Type::GetVoidType()) {}

        virtual ~Value() = default;

        Module& getModule() const { return mModule; }
        Type* getType() const { return mType; }
        virtual std::vector<std::reference_wrapper<Value*>> getOperands() { return {}; }
        virtual bool isConstant() const { return false; }

        bool requiresVReg() const { return mRequiresVReg; }
        void setPreferredRegister(int id) { mPreferredRegister = id; }

        virtual std::string identifier() const = 0;

        virtual void accept(Visitor& visitor) = 0;

    protected:
        Module& mModule;
        Type* mType;

        std::optional<bibbleasm::Operand> mEmittedValue;

        std::pair<int, int> mInterval{-1, -1};
        int mId = 0x69696969;

        VReg* mVReg = nullptr;
        bool mRequiresVReg = true;
        std::vector<VReg*> mDisallowedVRegs;
        std::vector<int> mDisallowedRegisters; // real registers
        bool mRegisterSmashesDone = false;
        int mPreferredRegister = -1; // real register
    };

    using ValuePtr = std::unique_ptr<Value>;
}

#endif // BIBBLIR_IR_VALUE_H

