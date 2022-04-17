#pragma once

#include <cstdint>
#include <memory>

#include "../Name.h"
#include "IO/Archive/Archive.h"

namespace Satisfactory3DMap {

    class ArrayVisitor;

    class Array {
    public:
        static std::unique_ptr<Array> create(const FName& array_type, Archive& ar);

        explicit Array(FName array_type) : array_type_(std::move(array_type)) {}
        virtual ~Array() = default;

        virtual void serialize(Archive& ar) = 0;

        virtual void accept(ArrayVisitor& v) = 0;

        [[nodiscard]] const FName& type() const {
            return array_type_;
        }

    protected:
        FName array_type_;
    };
} // namespace Satisfactory3DMap
