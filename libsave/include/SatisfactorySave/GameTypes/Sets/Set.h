#pragma once

#include <memory>

#include "../../IO/Archive/Archive.h"
#include "../Misc/Name.h"

namespace Satisfactory3DMap {

    class SetVisitor;

    class Set {
    public:
        static std::unique_ptr<Set> create(const FName& set_type, const FName& name, const std::string& parentClassName,
            Archive& ar);

        explicit Set(FName set_type) : set_type_(std::move(set_type)) {}
        virtual ~Set() = default;

        virtual void serialize(Archive& ar) = 0;

        virtual void accept(SetVisitor& v) = 0;

        [[nodiscard]] const FName& type() const {
            return set_type_;
        }

    protected:
        FName set_type_;
    };
} // namespace Satisfactory3DMap
