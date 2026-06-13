// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/optimizer/regalloc/vreg.h"

namespace bibblir {
    VReg::VReg(int id, uint16_t actual)
        : mId(id)
        , mUses(0)
        , mActualRegister(actual) {}

    int VReg::getId() const {
        return mId;
    }

    int VReg::getUses() const {
        return mUses;
    }

    uint16_t VReg::getActualRegister() const {
        return mActualRegister;
    }

    bibbleasm::Register VReg::toOperand() const {
        return bibbleasm::Register(mActualRegister);
    }
}
