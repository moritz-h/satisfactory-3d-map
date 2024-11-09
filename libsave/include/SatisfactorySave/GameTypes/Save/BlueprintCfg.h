#pragma once

#include <filesystem>

#include "../FactoryGame/FGFactoryBlueprintTypes.h"
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
