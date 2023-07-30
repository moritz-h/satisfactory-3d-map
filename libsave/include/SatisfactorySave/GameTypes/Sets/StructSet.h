#pragma once

#include <string>
#include <vector>

#include "../Structs/Base/Struct.h"
#include "Base/Set.h"

namespace SatisfactorySave {

    class StructSet : public Set {
    public:
        static constexpr std::string_view TypeName = "StructProperty";

        StructSet(const FName& name, const std::string& parentClassName);

        void serialize(Archive& ar) override;

        void accept(SetVisitor& v) override;

        [[nodiscard]] const FName& structName() const {
            return struct_name_;
        }

        std::vector<std::unique_ptr<Struct>> Set;

    protected:
        FName struct_name_;
    };
} // namespace SatisfactorySave
