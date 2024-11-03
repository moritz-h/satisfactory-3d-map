#pragma once

#include <string>

#include "../Structs/Base/Struct.h"
#include "Base/SetImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API StructSet final : public SetImplBase<StructSet, std::shared_ptr<Struct>> {
    public:
        static constexpr std::string_view TypeName = "StructProperty";

        static std::string structNameLookup(const FName& name, const std::string& parentClassName);

        explicit StructSet(FName struct_name);

        void serialize(Archive& ar) override;

        [[nodiscard]] const FName& getStructName() const {
            return struct_name_;
        }

    protected:
        FName struct_name_;
    };
} // namespace SatisfactorySave
