#ifndef SATISFACTORYSAVEGAME_SAVEOBJECTBASE_H
#define SATISFACTORYSAVEGAME_SAVEOBJECTBASE_H

#include <istream>
#include <string>

namespace SatisfactorySaveGame {

    // FObjectBaseSaveHeader
    class SaveObjectBase {
    public:
        SaveObjectBase(int32_t type, std::istream& stream);

    private:
        int32_t type_;
        std::string type_path_;
        std::string root_object_;
        std::string instance_name_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_SAVEOBJECTBASE_H
