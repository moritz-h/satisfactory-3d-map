#pragma once

#include <filesystem>

#include "../IO/Archive/IStreamArchive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API IoStoreFile {
    public:
        explicit IoStoreFile(const std::filesystem::path& path);

    private:
        std::unique_ptr<IFStreamArchive> ioStoreAr_;
    };
} // namespace SatisfactorySave
