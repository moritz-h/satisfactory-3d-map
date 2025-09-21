#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../GameTypes/UE/CoreUObject/UObject/Object.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    std::shared_ptr<UObject> AssetUObjectFactory(const std::string& class_name);

    class SATISFACTORYSAVE_API AssetExport {
    public:
        std::shared_ptr<UObject> Object;
        std::vector<char> BinaryClassData;
    };
} // namespace SatisfactorySave
