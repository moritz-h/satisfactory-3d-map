#pragma once

#include <string>
#include <vector>

#include "../../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    // UE return type is TArray<FDisplayNameEntryId>, but use string here for name entries.
    SATISFACTORYSAVE_API std::vector<std::string> LoadNameBatch(Archive& ar);
} // namespace SatisfactorySave
