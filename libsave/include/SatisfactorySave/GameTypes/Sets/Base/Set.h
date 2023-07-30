#pragma once

#include <memory>

#include "../../../IO/Archive/Archive.h"
#include "../../UE/UObject/Name.h"

namespace SatisfactorySave {

    class SetVisitor;

    class Set {
    public:
        static std::unique_ptr<Set> create(const FName& set_type, const FName& name, const std::string& parentClassName,
            Archive& ar);

        Set() = default;
        virtual ~Set() = default;

        virtual void serialize(Archive& ar) = 0;

        virtual void accept(SetVisitor& v) = 0;
    };
} // namespace SatisfactorySave
