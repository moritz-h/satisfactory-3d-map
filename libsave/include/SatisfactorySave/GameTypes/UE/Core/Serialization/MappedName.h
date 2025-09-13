#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "../../../../IO/Archive/Archive.h"
#include "../UObject/NameTypes.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    class SATISFACTORYSAVE_API FMappedName {
    public:
        static constexpr uint32_t InvalidIndex = ~uint32_t(0);
        static constexpr uint32_t IndexBits = 30u;
        static constexpr uint32_t IndexMask = (1u << IndexBits) - 1u;

        [[nodiscard]] inline uint32_t GetIndex() const {
            return Index & IndexMask;
        }

        [[nodiscard]] inline uint32_t GetNumber() const {
            return Number;
        }

        void serialize(Archive& ar) {
            ar << Index;
            ar << Number;
        }

    private:
        uint32_t Index = InvalidIndex;
        uint32_t Number = InvalidIndex;
    };

    class SATISFACTORYSAVE_API FNameMap {
    public:
        [[nodiscard]] inline int32_t Num() const {
            return static_cast<int32_t>(NameEntries.size());
        }

        [[nodiscard]] FName GetName(const FMappedName& MappedName) const {
            return FName(NameEntries[MappedName.GetIndex()], MappedName.GetNumber());
        }

        [[nodiscard]] const std::vector<std::string>& getNameEntries() const {
            return NameEntries;
        }

        void serialize(Archive& ar);

        [[nodiscard]] std::string toString() const;

    private:
        std::vector<std::string> NameEntries; // UE type is FDisplayNameEntryId, but use string here
    };
} // namespace SatisfactorySave
