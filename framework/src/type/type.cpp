// Copyright 2026 Jannik Laugmand Bülow

#include "BibblIR/type/type.h"
#include "BibblIR/type/void_type.h"

#include <functional>
#include <memory>
#include <vector>

#include "BibblIR/type/integer_type.h"

namespace bibblir {
    std::vector<std::unique_ptr<Type>> types;

    template<class T, auto Predicate, auto Make, class... Args>
    constexpr Type* GetType(Args&&... args) {
        for (const auto& type : types) {
            if (T* targetType = dynamic_cast<T*>(type.get())) {
                if (Predicate(targetType, std::forward<Args>(args)...)) {
                    return targetType;
                }
            }
        }

        types.push_back(Make(std::forward<Args>(args)...));
        return types.back().get();
    }

    template<class T>
    constexpr Type* GetType() {
        return GetType<
            T,
            [](T* value) { return true; },
            [] { return std::make_unique<T>(); }
        >();
    }

    Type* Type::GetVoidType() {
        return GetType<VoidType>();
    }

    Type* Type::GetIntegerType(int bytes) {
        return GetType<
            IntegerType,
            [](IntegerType* value, int bytes) { return value->getSizeInBytes() == bytes; },
            [](int bytes) { return std::make_unique<IntegerType>(bytes); }
        >(bytes);
    }
}
