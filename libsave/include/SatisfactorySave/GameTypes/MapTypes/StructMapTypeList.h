#pragma once

#include "../Structs/Base/Struct.h"
#include "Base/MapTypeListImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API StructMapTypeList final
        : public MapTypeListImplBase<StructMapTypeList, std::shared_ptr<Struct>> {
    public:
        static constexpr std::string_view TypeName = "StructProperty";

        static std::string structNameLookup(const FName& name, const std::string& parentClassName, bool isKey);

        explicit StructMapTypeList(FName struct_name);

        void serializeEntry(Archive& ar, std::size_t i) override;

        [[nodiscard]] const FName& getStructName() const {
            return struct_name_;
        }

    protected:
        FName struct_name_;
    };
} // namespace SatisfactorySave
