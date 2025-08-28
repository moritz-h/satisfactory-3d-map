#pragma once

#include <string>
#include <vector>

#include "AssetFile.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    class SATISFACTORYSAVE_API AbstractPakFile {
    public:
        virtual ~AbstractPakFile() = default;

        [[nodiscard]] virtual std::vector<std::string> getAllAssetFilenames() const = 0;

        [[nodiscard]] virtual bool containsAssetFilename(const std::string& filename) const = 0;

        virtual std::vector<char> readAssetFileContent(const std::string& filename) = 0;

        AssetFile readAsset(const std::string& filename);
    };
} // namespace SatisfactorySave
