#ifndef SATISFACTORYSAVEGAME_SAVEOBJECT_H
#define SATISFACTORYSAVEGAME_SAVEOBJECT_H

#include <istream>
#include <string>

namespace SatisfactorySaveGame {

    class SaveObject {
    public:
        SaveObject(int32_t type, std::istream& stream);

    private:
        int32_t type_;
        std::string type_path_;
        std::string root_object_;
        std::string instance_name_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_SAVEOBJECT_H
