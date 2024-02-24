#include "SplineData.h"

#include "SatisfactorySave/GameTypes/Arrays/StructArray.h"
#include "SatisfactorySave/GameTypes/Properties/ArrayProperty.h"
#include "SatisfactorySave/GameTypes/Properties/StructProperty.h"
#include "SatisfactorySave/GameTypes/Structs/PropertyStruct.h"
#include "SatisfactorySave/GameTypes/Structs/VectorStruct.h"

#include "Utils/GLMUtil.h"

namespace {
    struct SplinePointData {
        glm::vec3 location;
        glm::vec3 arriveTangent;
        glm::vec3 leaveTangent;
    };

    SatisfactorySave::ArrayProperty& getSplineDataProperty(const SatisfactorySave::PropertyList& properties) {
        for (const auto& p : properties) {
            if (p->name() == "mSplineData" && p->type() == SatisfactorySave::ArrayProperty::TypeName) {
                return dynamic_cast<SatisfactorySave::ArrayProperty&>(*p);
            }
        }

        throw std::runtime_error("mSplineData property missing!");
    }

    std::vector<SplinePointData> getSplineData(const SatisfactorySave::SaveActor& a) {
        auto& ap = getSplineDataProperty(a.Properties);
        if (ap.arrayType() != SatisfactorySave::StructProperty::TypeName) {
            throw std::runtime_error("Expect StructProperty!");
        }

        auto& sa = dynamic_cast<SatisfactorySave::StructArray&>(*ap.array());
        if (sa.structName() != "SplinePointData") {
            throw std::runtime_error("Expect SplinePointData!");
        }

        std::vector<SplinePointData> result;
        for (const auto& s : sa.Values) {
            const auto& ps = dynamic_cast<SatisfactorySave::PropertyStruct&>(*s);
            if (ps.Data.size() != 3) {
                throw std::runtime_error("Unexpected struct size!");
            }
            const auto& locationStruct = *dynamic_cast<const SatisfactorySave::StructProperty&>(ps.Data.at(0)).value();
            const auto& arriveTangentStruct =
                *dynamic_cast<const SatisfactorySave::StructProperty&>(ps.Data.at(1)).value();
            const auto& leaveTangentStruct =
                *dynamic_cast<const SatisfactorySave::StructProperty&>(ps.Data.at(2)).value();

            SplinePointData data;
            data.location =
                Satisfactory3DMap::glmCast(dynamic_cast<const SatisfactorySave::VectorStruct&>(locationStruct).Data);
            data.arriveTangent = Satisfactory3DMap::glmCast(
                dynamic_cast<const SatisfactorySave::VectorStruct&>(arriveTangentStruct).Data);
            data.leaveTangent = Satisfactory3DMap::glmCast(
                dynamic_cast<const SatisfactorySave::VectorStruct&>(leaveTangentStruct).Data);

            // transform to right-handed coords in meter
            data.location *= glm::vec3(0.01f, -0.01f, 0.01f);
            data.leaveTangent *= glm::vec3(0.01f, -0.01f, 0.01f);
            data.arriveTangent *= glm::vec3(0.01f, -0.01f, 0.01f);
            // transform to world-coords
            const auto location_world =
                (glm::translate(glm::mat4(1.0f), data.location) * Satisfactory3DMap::glmCast(a.Transform))[3];
            data.location = glm::vec3(location_world) / location_world.w;

            // Subtract actor position, will be later added in shader from global transformation list.
            // This allows updating the position independently of spline data.
            data.location -= glm::vec3(Satisfactory3DMap::glmCast(a.Transform) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

            result.emplace_back(data);
        }
        return result;
    }

    float splineLength(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& t0, const glm::vec3& t1) {
        // Use formula from:
        // https://github.com/EpicGames/UnrealEngine/blob/4.25.4-release/Engine/Source/Runtime/Engine/Private/Components/SplineComponent.cpp#L228-L282
        // https://en.wikipedia.org/wiki/Gauss%E2%80%93Legendre_quadrature

        static const std::pair<float, float> legendreGaussCoefficients[] = {
            // clang-format off
            { 0.0f,        0.5688889f},
            {-0.5384693f,  0.47862867f},
            { 0.5384693f,  0.47862867f},
            {-0.90617985f, 0.23692688f},
            { 0.90617985f, 0.23692688f}
            // clang-format on
        };

        const glm::vec3 coeff1 = ((p0 - p1) * 2.0f + t0 + t1) * 3.0f;
        const glm::vec3 coeff2 = (p1 - p0) * 6.0f - t0 * 4.0f - t1 * 2.0f;
        const glm::vec3 coeff3 = t0;

        float length = 0.0f;
        for (const auto& legendreGaussCoefficient : legendreGaussCoefficients) {
            const float alpha = 0.5f * (1.0f + legendreGaussCoefficient.first);
            const glm::vec3 derivative = (coeff1 * alpha + coeff2) * alpha + coeff3;
            length += glm::length(derivative) * legendreGaussCoefficient.second;
        }
        return length * 0.5f;
    }
} // namespace

Satisfactory3DMap::SplineData::SplineData(const SatisfactorySave::SaveActor& actor) {
    auto splineData = getSplineData(actor);

    length_ = 0.0f;
    for (std::size_t i = 1; i < splineData.size(); i++) {
        const auto p0 = splineData[i - 1].location;
        const auto p1 = splineData[i].location;
        const auto t0 = splineData[i - 1].leaveTangent;
        const auto t1 = splineData[i].arriveTangent;

        const float length = splineLength(p0, p1, t0, t1);
        length_ += length;

        SplineSegmentGpu segment;
        segment.p0 = glm::vec4(p0, 0.0f);
        segment.p1 = glm::vec4(p1, 0.0f);
        segment.tangent0 = glm::vec4(t0, 0.0f);
        segment.tangent1 = glm::vec4(t1, 0.0f);
        segment.len = length;
        splineSegments_.emplace_back(segment);
    }
}
