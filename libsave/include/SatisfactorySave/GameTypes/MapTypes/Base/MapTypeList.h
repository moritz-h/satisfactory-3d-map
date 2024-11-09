#pragma once

#include <memory>

#include "../../../IO/Archive/Archive.h"
#include "../../UE/Core/UObject/NameTypes.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class MapTypeListVisitor;

    class SATISFACTORYSAVE_API MapTypeList {
    public:
        static std::shared_ptr<MapTypeList> create(const FName& map_type, const FName& name,
            const std::string& parentClassName, bool isKey);

        MapTypeList() = default;
        virtual ~MapTypeList() = default;
        MapTypeList(const MapTypeList&) = default;
        MapTypeList& operator=(const MapTypeList&) = default;
        MapTypeList(MapTypeList&&) = default;
        MapTypeList& operator=(MapTypeList&&) = default;

        [[nodiscard]] virtual std::shared_ptr<MapTypeList> clone() const = 0;

        virtual void serializeEntry(Archive& ar, std::size_t i) = 0;

        virtual void accept(MapTypeListVisitor& v) = 0;

        [[nodiscard]] virtual std::size_t size() const = 0;

        virtual void resize(std::size_t s) = 0;
    };
} // namespace SatisfactorySave
