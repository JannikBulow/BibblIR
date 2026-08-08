// Copyright 2026 Jannik Laugmand Bülow

#ifndef BIBBLIR_PASS_PROPERTY_H
#define BIBBLIR_PASS_PROPERTY_H

namespace bibblir {
    enum class IRProperty {
        NoCriticalEdges,
        CanonicalCFG,
        AllocatedRegisters,

        EmittedBytecode,
    };
}

#endif //BIBBLIR_PASS_PROPERTY_H
