// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/ir/class.h"

#include "BibblIR/visitor/visitor.h"

#include "BibblIR/module.h"

#include <cassert>

namespace bibblir {
    Class* Class::Create(Module& module, std::string name) {
        Class* clas = new Class(module, std::move(name));
        module.insertGlobal(GlobalPtr(clas));
        return clas;
    }

    Field* Class::getField(std::string_view name) const {
        for (const auto& field : mFields) {
            if (field->mName == name) {
                return field.get();
            }
        }
        return nullptr;
    }

    Field* Class::addField(Type* type, std::string name) {
        if (Field* field = getField(name)) {
            assert(field->getType() == type);
            return field;
        }

        mFields.emplace_back(new Field(this, type, std::move(name)));
        return mFields.back().get();
    }

    Method* Class::getMethod(std::string_view name) const {
        for (const auto& method : mMethods) {
            if (method->mName == name) {
                return method.get();
            }
        }
        return nullptr;
    }

    Method* Class::addMethod(FunctionType* type, std::string name, Value* impl) {
        if (Method* method = getMethod(name)) {
            assert(method->getType() == type);
            assert(method->mImpl == impl);
            return method;
        }

        mMethods.emplace_back(new Method(this, type, name, impl));
        return mMethods.back().get();
    }

    std::string Class::identifier() const {
        return "%" + mName;
    }

    void Class::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    Class::Class(Module& module, std::string name)
        : Global(module)
        , mName(std::move(name)) {
        mType = Type::GetClassType(module.getName(), mName);
    }
}
