// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/type/class_type.h"

namespace bibblir {
    ClassType::ClassType(std::vector<Type*> fields)
        : Type(8, "class { ")
        , mFields(std::move(fields)) {
        mOperandSize = bibbleasm::OperandSize::Long;

        for (Type* field : mFields) {
            mName += std::string(field->getName()) + " ";
        }
        mName += '}';
    }

    const std::vector<Type*>& ClassType::getFields() const {
        return mFields;
    }

    Type* ClassType::getField(unsigned int index) const {
        return mFields[index];
    }

    Type::ID ClassType::getID() const {
        return ID::Reference;
    }

    bool ClassType::isClassType() const {
        return true;
    }
}
