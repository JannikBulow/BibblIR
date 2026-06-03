// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/type/integer_type.h"
#include "BibblIR/type/void_type.h"

namespace bibblir {
    IntegerType::IntegerType(int bytes)
        : Type(bytes, "") {
        switch (bytes) {
            case 1:
                mName = "byte";
                mOperandSize = bibbleasm::OperandSize::Byte;
                break;
            case 2:
                mName = "short";
                mOperandSize = bibbleasm::OperandSize::Short;
                break;
            case 4:
                mName = "int";
                mOperandSize = bibbleasm::OperandSize::Int;
                break;
            case 8:
                mName = "long";
                mOperandSize = bibbleasm::OperandSize::Long;
                break;
            default:
                mName = "int" + std::to_string(bytes);
                break;
        }
    }

    bool IntegerType::isIntegerType() const {
        return true;
    }

    VoidType::VoidType()
        : Type(0, "void") {}

    bool VoidType::isVoidType() const {
        return true;
    }
}