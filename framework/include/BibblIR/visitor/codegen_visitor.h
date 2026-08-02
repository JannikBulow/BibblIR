// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_VISITOR_CODEGEN_VISITOR_H
#define BIBBLIR_VISITOR_CODEGEN_VISITOR_H

#include "BibblIR/visitor/visitor.h"

#include <BibbleASM/codegen/builder/module_builder.h>

#include <BibbleASM/instruction/builder.h>

#include <ostream>
#include <unordered_map>

namespace bibblir {
    class BIBBLIR_EXPORT CodegenVisitor : public Visitor {
    public:
        bibbleasm::Module buildModule();

        void printDisassembly(std::ostream& stream);

        void visit(Module& module) override;

        void visit(Class& clas) override;
        void visit(Field& field) override;
        void visit(Method& method) override;

        void visit(Function& function) override;

        void visit(ExternalFunction& function) override;

        void visit(BasicBlock& block) override;

        void visit(Argument& arg) override;

        void visit(ConstantBoolean& constant) override;
        void visit(ConstantInt& constant) override;

        void visit(BinaryInstruction& instruction) override;
        void visit(BranchInstruction& instruction) override;
        void visit(CallInstruction& instruction) override;
        void visit(FieldInstruction& instruction) override;
        void visit(IntCastInstruction& instruction) override;
        void visit(LoadInstruction& instruction) override;
        void visit(NewInstruction& instruction) override;
        void visit(PhiInstruction& instruction) override;
        void visit(ReturnInstruction& instruction) override;
        void visit(StoreInstruction& instruction) override;
        void visit(UnaryInstruction& instruction) override;

    private:
        struct TwoString {
            std::string first;
            std::string second;

            bool operator==(const TwoString& other) const {
                return first == other.first && second == other.second;
            }
        };

        struct TwoStringHash {
            std::size_t operator()(const TwoString& s) const {
                auto h1 = std::hash<std::string>()(s.first);
                auto h2 = std::hash<std::string>()(s.second);
                return h1 ^ (h2 << 1);
            }
        };

        struct ThreeString {
            std::string first;
            std::string second;
            std::string third;

            bool operator==(const ThreeString& other) const {
                return first == other.first && second == other.second && third == other.third;
            }
        };

        struct ThreeStringHash {
            std::size_t operator()(const ThreeString& s) const {
                auto h1 = std::hash<std::string>()(s.first);
                auto h2 = std::hash<std::string>()(s.second);
                auto h3 = std::hash<std::string>()(s.third);

                std::size_t seed = h1;
                seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                seed ^= h3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);

                return seed;
            }
        };

        const std::string* mModuleName = nullptr;

        bibbleasm::ModuleBuilder mBuilder;
        bibbleasm::ClassBuilder* mClassBuilder = nullptr;
        bibbleasm::InstructionBuilder* mInstBuilder = nullptr;

        std::unordered_map<std::string, bibbleasm::ConstantIndex> mStringConstants;
        std::unordered_map<std::string, bibbleasm::ConstantIndex> mModuleInfoConstants;
        std::unordered_map<TwoString, bibbleasm::ConstantIndex, TwoStringHash> mFunctionInfoConstants;
        std::unordered_map<TwoString, bibbleasm::ConstantIndex, TwoStringHash> mClassInfoConstants;
        std::unordered_map<ThreeString, bibbleasm::ConstantIndex, ThreeStringHash> mFieldInfoConstants;
        std::unordered_map<ThreeString, bibbleasm::ConstantIndex, ThreeStringHash> mMethodInfoConstants;

        bibbleasm::ConstantIndex getStringConstant(const std::string& str);
        bibbleasm::ConstantIndex getModuleInfoConstant(const std::string& name);
        bibbleasm::ConstantIndex getFunctionInfoConstant(const std::string& moduleName, const std::string& name);
        bibbleasm::ConstantIndex getClassInfoConstant(const std::string& moduleName, const std::string& name);
        bibbleasm::ConstantIndex getFieldInfoConstant(const std::string& moduleName, const std::string& className, const std::string& name);
        bibbleasm::ConstantIndex getMethodInfoConstant(const std::string& moduleName, const std::string& className, const std::string& name);
    };
}

#endif //BIBBLIR_VISITOR_CODEGEN_VISITOR_H
