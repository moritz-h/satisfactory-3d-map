#pragma once

#include <filesystem>

#include "../UE/Satisfactory/BlueprintTypes.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    // .sbpcfg file
    class SATISFACTORYSAVE_API BlueprintCfg {
    public:
        BlueprintCfg() = default;
        explicit BlueprintCfg(const std::filesystem::path& filepath);

        void save(const std::filesystem::path& filepath);

        FBlueprintRecord record;
    };
} // namespace SatisfactorySave
