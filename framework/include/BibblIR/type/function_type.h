// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_TYPE_FUNCTION_TYPE_H
#define BIBBLIR_TYPE_FUNCTION_TYPE_H

#include "BibblIR/type/type.h"

#include <vector>

namespace bibblir {
    class BIBBLIR_EXPORT FunctionType : public Type {
    public:
        FunctionType(Type* returnType, std::vector<Type*> argumentTypes);

        Type* getReturnType() const;
        const std::vector<Type*>& getArgumentTypes() const;

        ID getID() const override;

        bool isFunctionType() const override;

        static FunctionType* Create(Type* returnType, const std::vector<Type*>& argumentTypes);

    private:
        Type* mReturnType;
        std::vector<Type*> mArgumentTypes;
    };
}

#endif //BIBBLIR_TYPE_FUNCTION_TYPE_H
