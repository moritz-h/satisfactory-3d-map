#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <unordered_map>

#include "../GameTypes/UE/Core/IO/IoDirectoryIndex.h"
#include "../GameTypes/UE/Core/IO/IoStore.h"
#include "../IO/Archive/IStreamArchive.h"
#include "AbstractPakFile.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API DirectoryIndexReader {
    public:
        explicit DirectoryIndexReader(const FIoDirectoryIndexResource& res);

        [[nodiscard]] const std::unordered_map<std::string, uint32_t>& directoryEntries() const {
            return directoryEntries_;
        }

    private:
        void parseDir(uint32_t dir_idx, const std::string& path);

        void parseFile(uint32_t file_idx, const std::string& path);

        static constexpr uint32_t none = ~static_cast<uint32_t>(0);

        const FIoDirectoryIndexResource& res_;
        std::unordered_map<std::string, uint32_t> directoryEntries_;
    };

    class SATISFACTORYSAVE_API IoStoreFile : public AbstractPakFile {
    public:
        explicit IoStoreFile(const std::filesystem::path& path);

        [[nodiscard]] std::vector<std::string> getAllAssetFilenames() const override;

        [[nodiscard]] bool containsAssetFilename(const std::string& filename) const override {
            return dirIndex_ != nullptr ? dirIndex_->directoryEntries().contains(filename) : false;
        }

        std::vector<char> readAssetFileContent(const std::string& filename) override;

    private:
        FIoStoreTocResource utoc_;
        std::unique_ptr<DirectoryIndexReader> dirIndex_;
    };
} // namespace SatisfactorySave
