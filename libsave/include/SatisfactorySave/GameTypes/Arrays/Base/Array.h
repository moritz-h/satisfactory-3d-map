#pragma once

#include <memory>

#include "../../../IO/Archive/Archive.h"
#include "../../UE/UObject/Name.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class ArrayVisitor;

    class SATISFACTORYSAVE_API Array {
    public:
        static std::unique_ptr<Array> create(const FName& array_type, Archive& ar);

        Array() = default;
        virtual ~Array() = default;
        Array(const Array&) = default;
        Array& operator=(const Array&) = default;
        Array(Array&&) = default;
        Array& operator=(Array&&) = default;

        [[nodiscard]] virtual std::unique_ptr<Array> clone() const = 0;

        virtual void serialize(Archive& ar) = 0;

        virtual void accept(ArrayVisitor& v) = 0;
    };
} // namespace SatisfactorySave
