#pragma once

#include "../UE/Satisfactory/RailroadTrackPosition.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API RailroadTrackPositionStruct final
        : public StructImpl<RailroadTrackPositionStruct, FRailroadTrackPosition> {
    public:
        static constexpr std::string_view TypeName = "RailroadTrackPosition";

        using StructImpl<RailroadTrackPositionStruct, FRailroadTrackPosition>::StructImpl;
    };
} // namespace SatisfactorySave
