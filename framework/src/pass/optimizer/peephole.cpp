// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/pass/codegen/codegen.h"

#include "BibblIR/pass/optimizer/peephole.h"

#include "BibblIR/module.h"

#include <iostream>

namespace bibblir {
    PassID PeepholePass::getId() const {
        return GetPassID<PeepholePass>();
    }

    std::vector<IRProperty> PeepholePass::getProvidedProperties() const {
        return {IRProperty::EmittedBytecode};
    }

    std::vector<IRProperty> PeepholePass::getRequiredProperties() const {
        return {IRProperty::EmittedBytecode};
    }

    std::vector<PassID> PeepholePass::runAfter() const {
        return {GetPassID<CodegenPass>()};
    }

    void PeepholePass::run(Module& module) {
        for (auto& function : module.bytecodeModule().functions()) {
            run(function->assembler());
        }
    }

    void PeepholePass::run(bibbleasm::Assembler& assembler) {
        bool hasOptimized = false;
        auto& instructions = assembler.instructions();

        for (auto it = instructions.begin() + 1; it != instructions.end();) {
            bool iterate = true;
            UnreachableLabelLevel unreachableLabel = checkUnreachableLabel(assembler, *it);

            if (unreachableLabel != UnreachableLabelLevel::References) {
                it = eraseNode(instructions, it);
                if (unreachableLabel == UnreachableLabelLevel::Unreachable) {
                    while (it->type != bibbleasm::AssemblerNode::Label) {
                        it = eraseNode(instructions, it);
                    }
                }

                iterate = false;
                hasOptimized = true;
            }

            if (it - 1 < instructions.end()) {
                if (checkJmpLabel(*it, *(it + 1))) {
                    it = eraseNode(instructions, it);
                    iterate = false;
                    hasOptimized = true;
                }
            }

            if (iterate) ++it;
        }

        if (hasOptimized) {
            run(assembler);
        } else {
            std::cout << assembler.disassemble() << "\n";
        }
    }

    PeepholePass::UnreachableLabelLevel PeepholePass::checkUnreachableLabel(bibbleasm::Assembler& assembler, bibbleasm::AssemblerNode& node) {
        if (node.type != bibbleasm::AssemblerNode::Label) return UnreachableLabelLevel::References;

        std::string& label = node.label;

        for (const auto& node : assembler.instructions()) {
            if (node.type == bibbleasm::AssemblerNode::Insn) {
                for (const auto& operand : node.insn.operands()) {
                    if (const auto* labelOperand = std::get_if<bibbleasm::Label>(&operand)) {
                        if (labelOperand->name == label) {
                            return UnreachableLabelLevel::References;
                        }
                    }
                }
            }
        }

        if (mReferenceOptimizedLabels.contains(label)) return UnreachableLabelLevel::OptimizedReferences;
        return UnreachableLabelLevel::Unreachable;
    }

    bool PeepholePass::checkJmpLabel(bibbleasm::AssemblerNode& first, bibbleasm::AssemblerNode& second) {
        if (first.type != bibbleasm::AssemblerNode::Insn) return false;
        if (first.insn.getOpcode() != bibble::opcodes::JMP) return false;
        if (second.type != bibbleasm::AssemblerNode::Label) return false;

        for (const auto& operand : first.insn.operands()) {
            if (auto* labelOperand = std::get_if<bibbleasm::Label>(&operand)) {
                if (labelOperand->name == second.label) {
                    mReferenceOptimizedLabels.insert(labelOperand->name);
                    return true;
                }
            }
        }

        return false;
    }

    std::vector<bibbleasm::AssemblerNode>::iterator PeepholePass::eraseNode(std::vector<bibbleasm::AssemblerNode>& nodes, std::vector<bibbleasm::AssemblerNode>::iterator it) {
        return nodes.erase(it);
    }
}
