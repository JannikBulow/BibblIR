// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/constant/constant_boolean.h"
#include "BibblIR/ir/constant/constant_int.h"

#include "BibblIR/ir/instruction/binary_instruction.h"
#include "BibblIR/ir/instruction/branch_instruction.h"
#include "BibblIR/ir/instruction/call_instruction.h"
#include "BibblIR/ir/instruction/load_instruction.h"
#include "BibblIR/ir/instruction/phi_instruction.h"
#include "BibblIR/ir/instruction/return_instruction.h"
#include "BibblIR/ir/instruction/unary_instruction.h"

#include "BibblIR/ir/basicblock.h"
#include "BibblIR/ir/builder.h"

namespace bibblir {
    BasicBlock* IRBuilder::getInsertPoint() const {
        return mInsertPoint;
    }

    void IRBuilder::setInsertPoint(BasicBlock* newInsertPoint) {
        mInsertPoint = newInsertPoint;
    }

    void IRBuilder::insertAfter(Value* value) {
        mInsertAfter = value;
    }

    ConstantBoolean* IRBuilder::createConstantBoolean(bool value) {
        ConstantBoolean* constant = new ConstantBoolean(mInsertPoint, value);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(constant));

        return constant;
    }

    ConstantInt* IRBuilder::createConstantInt(intmax_t value, Type* type) {
        ConstantInt* constant = new ConstantInt(mInsertPoint, value, type);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(constant));

        return constant;
    }

    BinaryInstruction* IRBuilder::createAdd(Value* left, Value* right) {
        BinaryInstruction* instruction = new BinaryInstruction(mInsertPoint, left, BinaryInstruction::ADD, right);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    BinaryInstruction* IRBuilder::createSub(Value* left, Value* right) {
        BinaryInstruction* instruction = new BinaryInstruction(mInsertPoint, left, BinaryInstruction::SUB, right);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    BinaryInstruction* IRBuilder::createMul(Value* left, Value* right) {
        BinaryInstruction* instruction = new BinaryInstruction(mInsertPoint, left, BinaryInstruction::MUL, right);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    BinaryInstruction* IRBuilder::createSDiv(Value* left, Value* right) {
        BinaryInstruction* instruction = new BinaryInstruction(mInsertPoint, left, BinaryInstruction::SDIV, right);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    BinaryInstruction* IRBuilder::createUDiv(Value* left, Value* right) {
        BinaryInstruction* instruction = new BinaryInstruction(mInsertPoint, left, BinaryInstruction::UDIV, right);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    BinaryInstruction* IRBuilder::createSMod(Value* left, Value* right) {
        BinaryInstruction* instruction = new BinaryInstruction(mInsertPoint, left, BinaryInstruction::UMOD, right);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    BinaryInstruction* IRBuilder::createUMod(Value* left, Value* right) {
        BinaryInstruction* instruction = new BinaryInstruction(mInsertPoint, left, BinaryInstruction::SMOD, right);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    BinaryInstruction* IRBuilder::createAnd(Value* left, Value* right) {
        BinaryInstruction* instruction = new BinaryInstruction(mInsertPoint, left, BinaryInstruction::AND, right);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    BinaryInstruction* IRBuilder::createOr(Value* left, Value* right) {
        BinaryInstruction* instruction = new BinaryInstruction(mInsertPoint, left, BinaryInstruction::OR, right);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    BinaryInstruction* IRBuilder::createXor(Value* left, Value* right) {
        BinaryInstruction* instruction = new BinaryInstruction(mInsertPoint, left, BinaryInstruction::XOR, right);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    BinaryInstruction* IRBuilder::createSHL(Value* left, Value* right) {
        BinaryInstruction* instruction = new BinaryInstruction(mInsertPoint, left, BinaryInstruction::SHL, right);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    BinaryInstruction* IRBuilder::createSHR(Value* left, Value* right) {
        BinaryInstruction* instruction = new BinaryInstruction(mInsertPoint, left, BinaryInstruction::SHR, right);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    BinaryInstruction* IRBuilder::createSAR(Value* left, Value* right) {
        BinaryInstruction* instruction = new BinaryInstruction(mInsertPoint, left, BinaryInstruction::SAR, right);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    BinaryInstruction* IRBuilder::createCmpEQ(Value* left, Value* right) {
        BinaryInstruction* instruction = new BinaryInstruction(mInsertPoint, left, BinaryInstruction::EQ, right);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    BinaryInstruction* IRBuilder::createCmpNE(Value* left, Value* right) {
        BinaryInstruction* instruction = new BinaryInstruction(mInsertPoint, left, BinaryInstruction::NE, right);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    BinaryInstruction* IRBuilder::createCmpLT(Value* left, Value* right) {
        BinaryInstruction* instruction = new BinaryInstruction(mInsertPoint, left, BinaryInstruction::LT, right);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    BinaryInstruction* IRBuilder::createCmpGT(Value* left, Value* right) {
        BinaryInstruction* instruction = new BinaryInstruction(mInsertPoint, left, BinaryInstruction::GT, right);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    BinaryInstruction* IRBuilder::createCmpLE(Value* left, Value* right) {
        BinaryInstruction* instruction = new BinaryInstruction(mInsertPoint, left, BinaryInstruction::LE, right);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    BinaryInstruction* IRBuilder::createCmpGE(Value* left, Value* right) {
        BinaryInstruction* instruction = new BinaryInstruction(mInsertPoint, left, BinaryInstruction::GE, right);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    UnaryInstruction* IRBuilder::createNeg(Value* value) {
        UnaryInstruction* instruction = new UnaryInstruction(mInsertPoint, value, UnaryInstruction::NEG);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    UnaryInstruction* IRBuilder::createAbs(Value* value) {
        UnaryInstruction* instruction = new UnaryInstruction(mInsertPoint, value, UnaryInstruction::ABS);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    UnaryInstruction* IRBuilder::createNot(Value* value) {
        UnaryInstruction* instruction = new UnaryInstruction(mInsertPoint, value, UnaryInstruction::NOT);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    BranchInstruction* IRBuilder::createBr(BasicBlock* destination) {
        BranchInstruction* instruction = new BranchInstruction(mInsertPoint, destination);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    BranchInstruction* IRBuilder::createCondBr(Value* condition, BasicBlock* trueBranch, BasicBlock* falseBranch) {
        BranchInstruction* instruction = new BranchInstruction(mInsertPoint, condition, trueBranch, falseBranch);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    LoadInstruction* IRBuilder::createLoad(Value* variable) {
        LoadInstruction* instruction = new LoadInstruction(mInsertPoint, variable);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    PhiInstruction* IRBuilder::createPhi(Type* type) {
        PhiInstruction* instruction = new PhiInstruction(mInsertPoint, type);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    CallInstruction* IRBuilder::createCall(Value* callee, std::vector<Value*> parameters) {
        CallInstruction* instruction = new CallInstruction(mInsertPoint, callee, parameters);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(instruction));

        return instruction;
    }

    ReturnInstruction* IRBuilder::createReturn(Value* returnValue) {
        ReturnInstruction* inst = new ReturnInstruction(mInsertPoint, returnValue);

        mInsertPoint->insertValue(mInsertAfter, ValuePtr(inst));

        return inst;
    }
}
