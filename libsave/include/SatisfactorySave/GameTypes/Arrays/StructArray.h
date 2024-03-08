#pragma once

#include <vector>

#include "../Properties/Base/PropertyTag.h"
#include "../Structs/Base/Struct.h"
#include "../UE/Misc/Guid.h"
#include "Base/Array.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API StructArray final : public Array {
    public:
        static constexpr std::string_view TypeName = "StructProperty";

        StructArray();
        StructArray(const StructArray& other);
        StructArray& operator=(const StructArray& other);
        StructArray(StructArray&&) = default;
        StructArray& operator=(StructArray&&) = default;

        [[nodiscard]] std::unique_ptr<Array> clone() const override;

        void serialize(Archive& ar) override;

        void accept(ArrayVisitor& v) override;

        [[nodiscard]] FName& name() {
            return inner_tag_.Name;
        }

        [[nodiscard]] FName& structName() {
            return inner_tag_.StructName;
        }

        [[nodiscard]] FGuid& structGuid() {
            return inner_tag_.StructGuid;
        }

        std::vector<std::unique_ptr<Struct>> Values;

    protected:
        PropertyTag inner_tag_;
    };
} // namespace SatisfactorySave
