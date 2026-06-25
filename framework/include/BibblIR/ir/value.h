// Copyright 2026 Jannik Laugmand Bülowvoid CodegenVisitor::visit(BinaryInstruction& instruction) {}

#ifndef BIBBLIR_IR_VALUE_H
#define BIBBLIR_IR_VALUE_H

#include "BibblIR/optimizer/regalloc/vreg.h"

#include "BibblIR/type/type.h"

#include "BibblIR/api.h"

#include <BibbleASM/instruction/operand.h>

#include <format>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

namespace bibblir {
    class Module;
    class Visitor;

    class BIBBLIR_EXPORT Value {
        friend class CodegenVisitor;
        friend class PrintVisitor;
        friend class RegAlloc;
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

        void forceRegister() {
            mRequiresVReg = true;
            mForceRegister = true;
        }

        std::string getName(int valueId) const {
            if (mVReg) {
                return std::format("VREG{}", mVReg->getId());
            } else {
                return std::format("{}", valueId);
            }
        }

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
        std::vector<uint16_t> mDisallowedRegisters; // real registers
        int mPreferredRegister = -1; // real register
        bool mForceRegister = false; // some instructions which needs its operands in a register, can force them in a register during construction
    };

    using ValuePtr = std::unique_ptr<Value>;
}

#endif // BIBBLIR_IR_VALUE_H

