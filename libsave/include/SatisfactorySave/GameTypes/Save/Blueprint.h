#pragma once

#include <filesystem>

#include "../FactoryGame/FGFactoryBlueprintTypes.h"
#include "SaveObject.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    // .sbp file
    class SATISFACTORYSAVE_API Blueprint {
    public:
        Blueprint() = default;
        explicit Blueprint(const std::filesystem::path& filepath);

        void save(const std::filesystem::path& filepath);

        FBlueprintHeader header;

        SaveObjectList saveObjects;
    };
} // namespace SatisfactorySave
