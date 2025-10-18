#pragma once

#include <cstddef>
#include <filesystem>
#include <memory>
#include <vector>

#include <glowl/glowl.h>

#include "SatisfactorySave/Pak/PakManager.h"

#include "MapLODReader.h"

namespace s = SatisfactorySave;

namespace Satisfactory3DMap {
    class MapLODglTFWriter {
    public:
        explicit MapLODglTFWriter(const std::shared_ptr<s::PakManager>& pakManager);
        ~MapLODglTFWriter() = default;

        void save(const std::filesystem::path& path);

    private:
        std::vector<std::byte> decompressTexture(GLuint compressedTex, int width, int height);

        MapLODReader reader_;
        std::unique_ptr<glowl::GLSLProgram> copyShader_;
    };
} // namespace Satisfactory3DMap
