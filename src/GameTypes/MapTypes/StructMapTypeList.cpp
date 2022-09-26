#include "StructMapTypeList.h"

#include "MapTypeListVisitor.h"

Satisfactory3DMap::StructMapTypeList::StructMapTypeList(FName type, const FName& name,
    const std::string& parentClassName)
    : MapTypeList(std::move(type)) {
    if (parentClassName == "/Game/FactoryGame/Events/BP_EventSubsystem.BP_EventSubsystem_C" &&
        (name == "mStoredCalendarData" || name == "mCalendarData")) {
        struct_name_.Name = "CalendarData";
    } else if (parentClassName == "/Script/FactoryGame.FGStatisticsSubsystem" && name == "mActorsBuiltCount") {
        struct_name_.Name = "ActorBuiltData";
    } else {
        // Unknown struct types will be parsed as property struct anyway,
        // provide type information in struct name for debug logging.
        struct_name_.Name = "MapType;" + parentClassName + ";" + name.toString();
    }
}

void Satisfactory3DMap::StructMapTypeList::accept(Satisfactory3DMap::MapTypeListVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::StructMapTypeList::serializeEntry(Archive& ar, std::size_t i) {
    if (ar.isIArchive()) {
        auto s = Struct::create(struct_name_, ar);
        list_.push_back(std::move(s));
    } else {
        ar << *list_[i];
    }
}
