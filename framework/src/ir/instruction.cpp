// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/instruction.h"

#include "BibblIR/ir/basicblock.h"

namespace bibblir {
    void Instruction::eraseFromParent() {
        mParent->eraseValue(this);
    }
}
