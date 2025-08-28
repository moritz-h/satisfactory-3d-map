#pragma once

#include <filesystem>

#include "../IO/Archive/IStreamArchive.h"
#include "AbstractPakFile.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API IoStoreFile : public AbstractPakFile {
    public:
        explicit IoStoreFile(const std::filesystem::path& path);

        [[nodiscard]] std::vector<std::string> getAllAssetFilenames() const override {
            return {};
        }

        [[nodiscard]] bool containsAssetFilename(const std::string& filename) const override {
            return false;
        }

        std::vector<char> readAssetFileContent(const std::string& filename) override {
            return {};
        }

    private:
        std::unique_ptr<IFStreamArchive> ioStoreAr_;
    };
} // namespace SatisfactorySave
