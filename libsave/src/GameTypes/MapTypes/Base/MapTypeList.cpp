#include "GameTypes/MapTypes/Base/MapTypeList.h"

#include "GameTypes/MapTypes/Base/MapTypeListAll.h"

std::unique_ptr<SatisfactorySave::MapTypeList> SatisfactorySave::MapTypeList::create(const FName& map_type,
    const FName& name, const std::string& parentClassName, bool isKey) {

    if (map_type == ByteMapTypeList::TypeName) {
        return std::make_unique<ByteMapTypeList>();
    } else if (map_type == EnumMapTypeList::TypeName) {
        return std::make_unique<EnumMapTypeList>();
    } else if (map_type == FloatMapTypeList::TypeName) {
        return std::make_unique<FloatMapTypeList>();
    } else if (map_type == IntMapTypeList::TypeName) {
        return std::make_unique<IntMapTypeList>();
    } else if (map_type == NameMapTypeList::TypeName) {
        return std::make_unique<NameMapTypeList>();
    } else if (map_type == ObjectMapTypeList::TypeName) {
        return std::make_unique<ObjectMapTypeList>();
    } else if (map_type == StructMapTypeList::TypeName) {
        auto struct_name = FName(StructMapTypeList::structNameLookup(name, parentClassName, isKey));
        return std::make_unique<StructMapTypeList>(std::move(struct_name));
    } else {
        throw std::runtime_error("Map type \"" + map_type + "\" not implemented!");
    }
}
