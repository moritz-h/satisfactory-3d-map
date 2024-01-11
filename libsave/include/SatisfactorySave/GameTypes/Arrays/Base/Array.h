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

        // Delete copy operators, to allow use of unique_ptr.
        Array(const Array&) = delete;
        Array& operator=(const Array&) = delete;
        Array(Array&&) = default;
        Array& operator=(Array&&) = default;

        virtual void serialize(Archive& ar) = 0;

        virtual void accept(ArrayVisitor& v) = 0;
    };
} // namespace SatisfactorySave
