#pragma once

#include <vector>

#include "../Properties/Base/PropertyTag.h"
#include "../Structs/Base/Struct.h"
#include "../UE/Core/Misc/Guid.h"
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

        [[nodiscard]] std::shared_ptr<Array> clone() const override;

        void serialize(Archive& ar) override;

        void accept(ArrayVisitor& v) override;

        [[nodiscard]] FName& Name() {
            return inner_tag_.Name;
        }

        [[nodiscard]] FName& StructName() {
            return inner_tag_.StructName;
        }

        [[nodiscard]] FGuid& StructGuid() {
            return inner_tag_.StructGuid;
        }

        std::vector<std::shared_ptr<Struct>> Values;

    protected:
        PropertyTag inner_tag_;
    };
} // namespace SatisfactorySave
