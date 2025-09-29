#pragma once

#include <vector>

#include <implot3d.h>

#include "SatisfactorySave/GameTypes/UE/Engine/Engine/StaticMesh.h"

namespace s = SatisfactorySave;

namespace Satisfactory3DMap {
    class PlotStaticMesh {
    public:
        explicit PlotStaticMesh(const s::UStaticMesh& staticMesh);

        [[nodiscard]] const std::vector<ImPlot3DPoint>& vertices() const {
            return vertices_;
        }

        [[nodiscard]] const std::vector<unsigned int>& indices() const {
            return indices_;
        }

        [[nodiscard]] const ImPlot3DBox& aabb() const {
            return aabb_;
        }

        [[nodiscard]] const ImPlot3DBox& cube() const {
            return cube_;
        }

    protected:
        std::vector<ImPlot3DPoint> vertices_;
        std::vector<unsigned int> indices_;

        ImPlot3DBox aabb_;
        ImPlot3DBox cube_;
    };
} // namespace Satisfactory3DMap
