#pragma once

#include <filesystem>
#include <memory>
#include <vector>

#include "../UE/Satisfactory/BlueprintTypes.h"
#include "SaveObjectBase.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    // .sbp file
    class SATISFACTORYSAVE_API Blueprint {
    public:
        Blueprint() = default;
        explicit Blueprint(const std::filesystem::path& filepath);

        void save(const std::filesystem::path& filepath);

        FBlueprintHeader header;

        std::vector<std::shared_ptr<SaveObjectBase>> saveObjects;
    };
} // namespace SatisfactorySave
