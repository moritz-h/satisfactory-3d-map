#ifndef SATISFACTORY3DMAP_RAILROADTRACKPOSITIONSTRUCT_H
#define SATISFACTORY3DMAP_RAILROADTRACKPOSITIONSTRUCT_H

#include "../ObjectReference.h"
#include "Struct.h"

namespace Satisfactory3DMap {

    class RailroadTrackPositionStruct : public Struct {
    public:
        RailroadTrackPositionStruct(std::string struct_name, std::istream& stream);

        void accept(StructVisitor& v) override;

    protected:
        ObjectReference ref_;
        float offset_;
        float forward_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_RAILROADTRACKPOSITIONSTRUCT_H
