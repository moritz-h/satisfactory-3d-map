#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "SatisfactorySave/GameTypes/Save/SaveActor.h"

namespace Satisfactory3DMap {

    struct SplineSegmentGpu {
        glm::vec4 p0;
        glm::vec4 p1;
        glm::vec4 tangent0;
        glm::vec4 tangent1;
        float len;
        float _padding_[3];
    };
    static_assert(sizeof(SplineSegmentGpu) == 5 * 4 * sizeof(float), "SplineSegmentGpu: Alignment issue!");

    class SplineData {
    public:
        explicit SplineData(const SatisfactorySave::SaveActor& actor);
        ~SplineData() = default;

        [[nodiscard]] const std::vector<SplineSegmentGpu>& splineSegments() const {
            return splineSegments_;
        }

        [[nodiscard]] float length() const {
            return length_;
        }

    protected:
        std::vector<SplineSegmentGpu> splineSegments_;
        float length_;
    };
} // namespace Satisfactory3DMap
