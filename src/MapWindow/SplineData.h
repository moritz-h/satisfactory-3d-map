#ifndef SATISFACTORY3DMAP_SPLINEDATA_H
#define SATISFACTORY3DMAP_SPLINEDATA_H

#include <cstdint>

#include <glm/glm.hpp>

#include "SaveGame/Objects/SaveActor.h"

namespace Satisfactory3DMap {

    enum class SplineModelType : int32_t {
        None = 0,
        ConveyorBelt = 1,
        Pipe = 2,
        Hyper = 3,
        Track = 4,
    };

    // TODO
    struct SplineSegmentGpu {
        glm::vec4 p0;
        glm::vec4 p1;
        glm::vec4 tangent0;
        glm::vec4 tangent1;
        int32_t id;
        int32_t type;
        int32_t _padding_[2];
    };
    static_assert(
        sizeof(SplineSegmentGpu) == 16 * sizeof(float) + 4 * sizeof(int32_t), "SplineSegmentGpu: Alignment issue!");

    class SplineData {
    public:
        SplineData(SplineModelType spline_type, const SaveActor& actor);
        ~SplineData() = default;

        // TODO
        std::vector<SplineSegmentGpu> splineSegments_;

    protected:
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_SPLINEDATA_H
