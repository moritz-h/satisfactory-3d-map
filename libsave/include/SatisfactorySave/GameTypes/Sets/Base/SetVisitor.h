#pragma once

#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class ObjectSet;
    class StructSet;
    class UInt32Set;

    class SATISFACTORYSAVE_API SetVisitor {
    public:
        virtual ~SetVisitor() = default;

        virtual void visit(ObjectSet& s) = 0;
        virtual void visit(StructSet& s) = 0;
        virtual void visit(UInt32Set& s) = 0;
    };
} // namespace SatisfactorySave
