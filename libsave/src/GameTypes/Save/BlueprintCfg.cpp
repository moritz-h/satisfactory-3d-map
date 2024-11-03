#include "GameTypes/Save/BlueprintCfg.h"

#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"

SatisfactorySave::BlueprintCfg::BlueprintCfg(const std::filesystem::path& filepath) {
    IFStreamArchive fileAr(filepath);
    fileAr << record;

    if (fileAr.tell() != fileAr.size()) {
        throw std::runtime_error("BlueprintCfg: Size check after parsing failed!");
    }
}

void SatisfactorySave::BlueprintCfg::save(const std::filesystem::path& filepath) {
    OFStreamArchive fileAr(filepath);
    fileAr << record;
}
