// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/external_class.h"

#include "BibblIR/visitor/visitor.h"

#include "BibblIR/type/class_type.h"

#include <cassert>
#include <format>

namespace bibblir {
    std::string_view ExternalClass::getModuleName() const {
        return mModuleName;
    }

    std::string_view ExternalClass::getName() const {
        return mName;
    }

    const std::vector<FieldPtr>& ExternalClass::getFields() const {
        return mFields;
    }

    Field* ExternalClass::getField(std::string_view name) const {
        for (const auto& field : mFields) {
            if (field->mName == name) {
                return field.get();
            }
        }
        return nullptr;
    }

    Field* ExternalClass::addField(Type* type, std::string name) {
        if (Field* field = getField(name)) {
            assert(field->getType() == type);
            return field;
        }

        mFields.emplace_back(new Field(this, type, std::move(name)));
        return mFields.back().get();
    }

    const std::vector<MethodPtr>& ExternalClass::getMethods() const {
        return mMethods;
    }

    Method* ExternalClass::getMethod(std::string_view name) const {
        for (const auto& method : mMethods) {
            if (method->mName == name) {
                return method.get();
            }
        }
        return nullptr;
    }

    Method* ExternalClass::addMethod(FunctionType* type, std::string name, Value* impl) {
        if (Method* method = getMethod(name)) {
            assert(method->getType() == type);
            assert(method->mImpl == impl);
            return method;
        }

        mMethods.emplace_back(new Method(this, type, std::move(name), impl));
        return mMethods.back().get();
    }

    std::string ExternalClass::identifier() const {
        return std::format("{}::{}", mModuleName, mName);
    }

    void ExternalClass::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    ExternalClass::ExternalClass(Module& module, std::string moduleName, std::string name)
        : AbstractClass(module)
        , mModuleName(std::move(moduleName))
        , mName(std::move(name)) {
        mType = ClassType::GetClassType(mModuleName, mName);
    }
}
