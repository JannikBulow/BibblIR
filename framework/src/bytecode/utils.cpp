// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/bytecode/utils.h"

#include <cassert>

namespace bibblir::bytecode {
    template<class... Ts>
    struct overloaded : Ts... {
        using Ts::operator()...;
    };

    template<class... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;

    void Move(bibbleasm::InstructionBuilder& builder, bibbleasm::Operand dst, bibbleasm::Operand src) {
        assert(std::holds_alternative<bibbleasm::Register>(dst));

        bibbleasm::Register dstReg = std::get<bibbleasm::Register>(dst);

        std::visit(overloaded{
            [&builder, &dstReg](const bibbleasm::Register& reg) {
                builder.mov(dstReg, reg);
            },
            [&builder, &dstReg](const bibbleasm::Immediate& imm) {
                builder.load_imm(dstReg, imm);
            },
            [&builder, &dstReg](const bibbleasm::ConstPoolIndex& constant) {
                builder.load_const(dstReg, constant);
            },

            [](const auto& value) {
                assert(false);
            }
        }, src);
    }
}
