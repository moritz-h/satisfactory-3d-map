#pragma once

namespace SatisfactorySave {

    class StructSet;
    class UInt32Set;

    class SetVisitor {
    public:
        virtual void visit(StructSet& a) = 0;
        virtual void visit(UInt32Set& a) = 0;
    };
} // namespace SatisfactorySave
