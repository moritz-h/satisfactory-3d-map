#ifndef SATISFACTORY3DMAP_RAILROADTRACKPOSITIONSTRUCT_H
#define SATISFACTORY3DMAP_RAILROADTRACKPOSITIONSTRUCT_H

#include "../ObjectReference.h"
#include "Struct.h"
#include "Utils/StreamUtils.h"

namespace Satisfactory3DMap {

    class RailroadTrackPositionStruct : public Struct {
    public:
        RailroadTrackPositionStruct(std::string struct_name, std::istream& stream) : Struct(std::move(struct_name)) {
            ref_ = ObjectReference(stream);
            offset_ = read<float>(stream);
            forward_ = read<float>(stream);
        }

    protected:
        ObjectReference ref_;
        float offset_;
        float forward_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_RAILROADTRACKPOSITIONSTRUCT_H
