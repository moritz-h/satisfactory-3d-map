#include "GameTypes/Save/BlueprintCfg.h"

#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"

SatisfactorySave::BlueprintCfg::BlueprintCfg(const std::filesystem::path& filepath) {
    IStreamArchive fileAr(filepath);
    fileAr << record;

    if (fileAr.tell() != fileAr.size()) {
        throw std::runtime_error("BlueprintCfg: Size check after parsing failed!");
    }
}

void SatisfactorySave::BlueprintCfg::save(const std::filesystem::path& filepath) {
    OStreamArchive fileAr(filepath);
    fileAr << record;
}
