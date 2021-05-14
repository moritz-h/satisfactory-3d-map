#include "SplineData.h"

#include "SaveGame/Types/Arrays/StructArray.h"
#include "SaveGame/Types/Properties/ArrayProperty.h"
#include "SaveGame/Types/Properties/StructProperty.h"
#include "SaveGame/Types/Structs/PropertyStruct.h"
#include "SaveGame/Types/Structs/VectorStruct.h"

namespace {
    struct SplinePointData {
        glm::vec3 location;
        glm::vec3 arriveTangent;
        glm::vec3 leaveTangent;
    };

    Satisfactory3DMap::ArrayProperty& getSplineDataProperty(
        const std::vector<std::unique_ptr<Satisfactory3DMap::Property>>& properties) {
        for (const auto& p : properties) {
            if (p->name() == "mSplineData" && p->type() == "ArrayProperty") {
                return dynamic_cast<Satisfactory3DMap::ArrayProperty&>(*p);
            }
        }

        throw std::runtime_error("mSplineData property missing!");
    }

    std::vector<SplinePointData> getSplineData(const Satisfactory3DMap::SaveActor& a) {
        auto& ap = getSplineDataProperty(a.properties());
        if (ap.arrayType() != "StructProperty") {
            throw std::runtime_error("Expect StructProperty!");
        }

        auto& sa = dynamic_cast<Satisfactory3DMap::StructArray&>(*ap.array());
        if (sa.structName() != "SplinePointData") {
            throw std::runtime_error("Expect SplinePointData!");
        }

        std::vector<SplinePointData> result;
        for (const auto& s : sa.array()) {
            const auto& ps = dynamic_cast<Satisfactory3DMap::PropertyStruct&>(*s);
            if (ps.properties().size() != 3) {
                throw std::runtime_error("Unexpected struct size!");
            }
            const auto& locationStruct = *dynamic_cast<Satisfactory3DMap::StructProperty&>(*ps.properties()[0]).value();
            const auto& arriveTangentStruct =
                *dynamic_cast<Satisfactory3DMap::StructProperty&>(*ps.properties()[1]).value();
            const auto& leaveTangentStruct =
                *dynamic_cast<Satisfactory3DMap::StructProperty&>(*ps.properties()[2]).value();

            SplinePointData data;
            data.location = dynamic_cast<const Satisfactory3DMap::VectorStruct&>(locationStruct).value();
            data.arriveTangent = dynamic_cast<const Satisfactory3DMap::VectorStruct&>(arriveTangentStruct).value();
            data.leaveTangent = dynamic_cast<const Satisfactory3DMap::VectorStruct&>(leaveTangentStruct).value();

            // transform to right-handed coords in meter
            data.location *= glm::vec3(0.01f, -0.01f, 0.01f);
            data.leaveTangent *= glm::vec3(0.01f, -0.01f, 0.01f);
            data.arriveTangent *= glm::vec3(0.01f, -0.01f, 0.01f);
            // transform to world-coords
            const auto location_world = (glm::translate(glm::mat4(1.0f), data.location) * a.transformation())[3];
            data.location = glm::vec3(location_world) / location_world.w;

            result.emplace_back(data);
        }
        return result;
    }

    float splineLength(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& t0, const glm::vec3& t1) {
        // Use formula from:
        // https://github.com/EpicGames/UnrealEngine/blob/4.25.4-release/Engine/Source/Runtime/Engine/Private/Components/SplineComponent.cpp#L228-L282

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

Satisfactory3DMap::SplineData::SplineData(
    Satisfactory3DMap::SplineModelType spline_type, const Satisfactory3DMap::SaveActor& actor) {
    auto splines = getSplineData(actor);
    for (std::size_t i = 1; i < splines.size(); i++) {
        auto p0 = splines[i - 1].location;
        auto p1 = splines[i].location;
        auto t0 = splines[i - 1].leaveTangent;
        auto t1 = splines[i].arriveTangent;

        // TODO
        float length = splineLength(p0, p1, t0, t1);

        // TODO
        SplineSegmentGpu segment;
        segment.p0 = glm::vec4(p0, 0.0f);
        segment.p1 = glm::vec4(p1, 0.0f);
        segment.tangent0 = glm::vec4(t0, 0.0f);
        segment.tangent1 = glm::vec4(t1, 0.0f);
        segment.id = actor.id();
        segment.type = static_cast<int32_t>(spline_type);
        splineSegments_.emplace_back(segment);
    }
}
