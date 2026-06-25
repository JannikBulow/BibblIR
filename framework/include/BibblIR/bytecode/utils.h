// Copyright 2026 Jannik Laugmand Bülow

// I sat here for like 5 minutes trying to think of some fuckass file names but couldn't

#ifndef BIBBLIR_BYTECODE_UTILS_H
#define BIBBLIR_BYTECODE_UTILS_H

#include "BibblIR/api.h"

#include <BibbleASM/instruction/builder.h>

#include <optional>

namespace bibblir::bytecode {
    BIBBLIR_EXPORT std::optional<bibbleasm::Instruction> BuildMove(bibbleasm::Operand dst, bibbleasm::Operand src);
    BIBBLIR_EXPORT void Move(bibbleasm::InstructionBuilder& builder, bibbleasm::Operand dst, bibbleasm::Operand src);
}

#endif //BIBBLIR_BYTECODE_UTILS_H
