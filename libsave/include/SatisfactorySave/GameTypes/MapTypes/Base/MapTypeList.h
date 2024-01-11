#pragma once

#include <memory>

#include "../../../IO/Archive/Archive.h"
#include "../../UE/UObject/Name.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class MapTypeListVisitor;

    class SATISFACTORYSAVE_API MapTypeList {
    public:
        static std::unique_ptr<MapTypeList> create(const FName& map_type, const FName& name,
            const std::string& parentClassName, bool isKey);

        MapTypeList() = default;
        virtual ~MapTypeList() = default;

        // Delete copy operators, to allow use of unique_ptr.
        MapTypeList(const MapTypeList&) = delete;
        MapTypeList& operator=(const MapTypeList&) = delete;
        MapTypeList(MapTypeList&&) = default;
        MapTypeList& operator=(MapTypeList&&) = default;

        virtual void serializeEntry(Archive& ar, std::size_t i) = 0;

        virtual void accept(MapTypeListVisitor& v) = 0;

        [[nodiscard]] virtual std::size_t size() const = 0;

        virtual void resize(std::size_t s) = 0;
    };
} // namespace SatisfactorySave
