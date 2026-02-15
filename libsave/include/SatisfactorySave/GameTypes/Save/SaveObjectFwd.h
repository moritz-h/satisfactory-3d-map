#pragma once

#include <memory>
#include <vector>

namespace SatisfactorySave {
    // Provide forward declarations to break cyclic dependency between SaveObject.h and FGSaveSession.h.
    class SaveObject;
    typedef std::shared_ptr<SaveObject> SaveObjectPtr;
    typedef std::vector<SaveObjectPtr> SaveObjectList;
} // namespace SatisfactorySave
