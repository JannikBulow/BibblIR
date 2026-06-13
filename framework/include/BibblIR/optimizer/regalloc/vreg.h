// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_OPTIMIZER_REGALLOC_VREG_H
#define BIBBLIR_OPTIMIZER_REGALLOC_VREG_H

#include "BibblIR/api.h"

#include <BibbleASM/instruction/operand.h>

namespace bibblir {
    class BIBBLIR_EXPORT VReg {
    public:
        VReg() = default;
        VReg(int id, uint16_t actual);

        int getId() const;
        int getUses() const;
        uint16_t getActualRegister() const;

        bibbleasm::Register toOperand() const;

    private:
        int mId;
        int mUses;

        bool mArgument;

        uint16_t mActualRegister;
    };
}

#endif //BIBBLIR_OPTIMIZER_REGALLOC_VREG_H
