// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/bytecode/utils.h"

#include <cassert>
#include <utility>

namespace bibblir::bytecode {
    template<class... Ts>
    struct overloaded : Ts... {
        using Ts::operator()...;
    };

    template<class... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;

    std::optional<bibbleasm::Instruction> BuildMove(bibbleasm::Operand dst, bibbleasm::Operand src) {
        assert(std::holds_alternative<bibbleasm::Register>(dst));

        bibbleasm::Register dstReg = std::get<bibbleasm::Register>(dst);

        std::optional<bibbleasm::Instruction> insn = std::nullopt;

        std::visit(overloaded{
            [&insn, &dstReg](const bibbleasm::Register& reg) {
                if (dstReg.index != reg.index) {
                    insn = bibbleasm::build::mov(dstReg, reg);
                }
            },
            [&insn, &dstReg](const bibbleasm::Immediate& imm) {
                insn = bibbleasm::build::load_imm(dstReg, imm);
            },
            [&insn, &dstReg](const bibbleasm::ConstPoolIndex& constant) {
                insn = bibbleasm::build::load_const(dstReg, constant);
            },

            [](const auto& value) {
                assert(false);
            }
        }, src);

        return insn;
    }

    void Move(bibbleasm::InstructionBuilder& builder, bibbleasm::Operand dst, bibbleasm::Operand src) {
        std::optional<bibbleasm::Instruction> insn = BuildMove(std::move(dst), std::move(src));
        if (insn) {
            builder.assembler().emit(*insn);
        }
    }
}
