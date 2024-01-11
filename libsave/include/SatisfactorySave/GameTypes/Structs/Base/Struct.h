#pragma once

#include <memory>
#include <utility>

#include "../../../IO/Archive/Archive.h"
#include "../../UE/UObject/Name.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class StructVisitor;

    class SATISFACTORYSAVE_API Struct {
    public:
        static std::unique_ptr<Struct> create(const FName& struct_name, Archive& ar);

        explicit Struct(FName name) : name_(std::move(name)){};
        virtual ~Struct() = default;

        virtual void serialize(Archive& ar) = 0;

        virtual void accept(StructVisitor& v) = 0;

        const FName& name() {
            return name_;
        }

    protected:
        FName name_;
    };
} // namespace SatisfactorySave
