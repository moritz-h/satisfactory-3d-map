#pragma once

#include <memory>
#include <utility>

#include "../../../IO/Archive/Archive.h"
#include "../../UE/Core/UObject/NameTypes.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class StructVisitor;

    class SATISFACTORYSAVE_API Struct {
    public:
        static std::shared_ptr<Struct> create(const FName& struct_name, Archive& ar);

        explicit Struct(FName name) : name_(std::move(name)) {}
        virtual ~Struct() = default;
        Struct(const Struct&) = default;
        Struct& operator=(const Struct&) = default;
        Struct(Struct&&) = default;
        Struct& operator=(Struct&&) = default;

        [[nodiscard]] virtual std::shared_ptr<Struct> clone() const = 0;

        virtual void serialize(Archive& ar) = 0;

        virtual void accept(StructVisitor& v) = 0;

        [[nodiscard]] const FName& getStructName() const {
            return name_;
        }

    protected:
        FName name_;
    };
} // namespace SatisfactorySave
