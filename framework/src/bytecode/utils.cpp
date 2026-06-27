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

    void Jump(bibbleasm::InstructionBuilder& builder, bibbleasm::Operand destination) {
        std::visit(overloaded{
            [&builder](const bibbleasm::Label& label) {
                builder.jmp(label);
            },
            [&builder](const bibbleasm::BranchOffset& offset) {
                builder.jmp(offset);
            },

            [](const auto& value) {
                assert(false);
            }
        }, destination);
    }

    void CondJump(bibbleasm::InstructionBuilder& builder, CondType condType, bibbleasm::Operand condition, bibbleasm::Operand destination) {
        assert(std::holds_alternative<bibbleasm::Register>(condition));

        bibbleasm::Register conditionReg = std::get<bibbleasm::Register>(condition);

#define GENERATE_SWITCH(dst) switch (condType) {\
    case CondType::EQ:\
        builder.jeq(conditionReg, dst);\
        break;\
    case CondType::NE:\
        builder.jne(conditionReg, dst);\
        break;\
    case CondType::LT:\
        builder.jlt(conditionReg, dst);\
        break;\
    case CondType::GT:\
        builder.jgt(conditionReg, dst);\
        break;\
    case CondType::LE:\
        builder.jle(conditionReg, dst);\
        break;\
    case CondType::GE:\
        builder.jge(conditionReg, dst);\
        break;\
    }

        std::visit(overloaded{
            [&builder, condType, &conditionReg](const bibbleasm::Label& label) {
                GENERATE_SWITCH(label);
            },
            [&builder, condType, &conditionReg](const bibbleasm::BranchOffset& offset) {
                GENERATE_SWITCH(offset);
            },

            [](const auto& value) {
                assert(false);
            }
        }, destination);
    }

#undef GENERATE_SWITCH
}
