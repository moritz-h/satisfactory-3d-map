#pragma once

#include <memory>

#include "../../../IO/Archive/IStreamArchive.h"
#include "../../UE/Core/UObject/NameTypes.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SetVisitor;

    class SATISFACTORYSAVE_API Set {
    public:
        static std::shared_ptr<Set> create(const FName& set_type, const FName& name, IStreamArchive& ar);

        Set() = default;
        virtual ~Set() = default;
        Set(const Set&) = default;
        Set& operator=(const Set&) = default;
        Set(Set&&) = default;
        Set& operator=(Set&&) = default;

        [[nodiscard]] virtual std::shared_ptr<Set> clone() const = 0;

        virtual void serialize(Archive& ar) = 0;

        virtual void accept(SetVisitor& v) = 0;
    };
} // namespace SatisfactorySave
