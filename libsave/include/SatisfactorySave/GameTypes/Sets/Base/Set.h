#pragma once

#include <memory>

#include "../../../IO/Archive/IStreamArchive.h"
#include "../../UE/UObject/Name.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SetVisitor;

    class SATISFACTORYSAVE_API Set {
    public:
        static std::unique_ptr<Set> create(const FName& set_type, const FName& name, IStreamArchive& ar);

        Set() = default;
        virtual ~Set() = default;

        // Delete copy operators, to allow use of unique_ptr.
        Set(const Set&) = delete;
        Set& operator=(const Set&) = delete;
        Set(Set&&) = default;
        Set& operator=(Set&&) = default;

        virtual void serialize(Archive& ar) = 0;

        virtual void accept(SetVisitor& v) = 0;
    };
} // namespace SatisfactorySave
