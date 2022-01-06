#ifndef SATISFACTORY3DMAP_RAILROADTRACKPOSITIONSTRUCT_H
#define SATISFACTORY3DMAP_RAILROADTRACKPOSITIONSTRUCT_H

#include "../ObjectReference.h"
#include "Struct.h"

namespace Satisfactory3DMap {

    class RailroadTrackPositionStruct : public Struct {
    public:
        RailroadTrackPositionStruct(std::string struct_name, std::istream& stream);

        void serialize(std::ostream& stream) const override;

        void accept(StructVisitor& v) override;

        const ObjectReference& ref() const {
            return ref_;
        }

        float offset() const {
            return offset_;
        }

        float forward() const {
            return forward_;
        }

    protected:
        ObjectReference ref_;
        float offset_;
        float forward_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_RAILROADTRACKPOSITIONSTRUCT_H
