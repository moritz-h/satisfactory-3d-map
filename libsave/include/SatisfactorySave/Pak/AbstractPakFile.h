#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../IO/IOStream.h"
#include "AssetFile.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    class PakManager;

    class SATISFACTORYSAVE_API AbstractPakFile : public std::enable_shared_from_this<AbstractPakFile> {
    protected:
        struct Private {
            explicit Private() = default;
        };

    public:
        virtual ~AbstractPakFile() = default;

        [[nodiscard]] virtual std::vector<std::string> getAllAssetFilenames() const = 0;

        [[nodiscard]] virtual bool containsAssetFilename(const std::string& filename) const = 0;

        virtual std::vector<char> readAssetFileContent(const std::string& filename) = 0;

        virtual std::unique_ptr<IStream> getAssetFileStream(const std::string& filename);

        AssetFile readAsset(const std::string& filename);

    protected:
        explicit AbstractPakFile(Private, std::shared_ptr<PakManager> pakManager)
            : pakManager_(std::move(pakManager)) {}

        std::shared_ptr<PakManager> pakManager_;
    };
} // namespace SatisfactorySave
