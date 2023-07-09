#pragma once

#include <memory>

#include "../../../IO/Archive/Archive.h"
#include "../../UE/UObject/Name.h"

namespace SatisfactorySave {

    class ArrayVisitor;

    class Array {
    public:
        static std::unique_ptr<Array> create(const FName& array_type, Archive& ar);

        explicit Array() = default;
        virtual ~Array() = default;

        virtual void serialize(Archive& ar) = 0;

        virtual void accept(ArrayVisitor& v) = 0;
    };
} // namespace SatisfactorySave
