#include "GameTypes/MapTypes/StructMapTypeList.h"

std::string SatisfactorySave::StructMapTypeList::structNameLookup(const FName& name, const std::string& parentClassName,
    bool isKey) {
    if (!isKey &&
        parentClassName == "/Game/FactoryGame/Buildable/Factory/TruckStation/Build_TruckStation.Build_TruckStation_C" &&
        name == "mDockingVehicleStatistics") {
        return "DockingVehicleStatistics";
    } else if (!isKey && parentClassName == "/Game/FactoryGame/Events/BP_EventSubsystem.BP_EventSubsystem_C" &&
               (name == "mStoredCalendarData" || name == "mCalendarData")) {
        return "CalendarData";
    } else if (!isKey && parentClassName == "/Game/FactoryGame/Events/BP_EventSubsystem.BP_EventSubsystem_C" &&
               name == "mCalendarsOpenedByPlayers") {
        return "PlayerStateSetWrapper";
    } else if (parentClassName == "/Script/FactoryGame.FGFoliageRemovalSubsystem" && name == "mSaveData") {
        if (isKey) {
            return "IntVector";
        } else {
            return "FoliageRemovalSaveDataPerCell";
        }
    } else if (parentClassName == "/Script/FactoryGame.FGFoliageRemovalSubsystem" && name == "mUnresolvedSaveData") {
        if (isKey) {
            return "IntVector";
        } else {
            return "FoliageRemovalUnresolvedSaveDataPerCell";
        }
    } else if (!isKey && parentClassName == "/Script/FactoryGame.FGStatisticsSubsystem" &&
               (name == "mItemsManuallyCraftedCount" || name == "mItemsPickedUp")) {
        return "MappedItemAmount";
    } else if (!isKey && parentClassName == "/Script/FactoryGame.FGStatisticsSubsystem" &&
               name == "mActorsBuiltCount") {
        return "ActorBuiltData";
    } else if (!isKey &&
               (parentClassName == "FoliageRemovalSaveDataPerCell" ||
                   parentClassName == "FoliageRemovalUnresolvedSaveDataPerCell") &&
               name == "SaveDataMap") {
        return "FoliageRemovalSaveDataForFoliageType";
    } else if (!isKey && parentClassName == "LBBalancerData" && name == "mIndexMapping") {
        // https://github.com/mklierman/SatisfactoryMods/blob/6a9ed9c315c8f58dbbda22efc788e66566c93321/LoadBalancers/Source/LoadBalancers/Public/LBBuild_ModularLoadBalancer.h#L92
        return "LBBalancerIndexing";
    } else {
        // Unknown struct types will be parsed as property struct anyway,
        // provide type information in struct name for debug logging.
        return "MapType;" + parentClassName + ";" + name.toString() + ";" + (isKey ? "key" : "val");
    }
}

SatisfactorySave::StructMapTypeList::StructMapTypeList(FName struct_name) : struct_name_(std::move(struct_name)) {}

void SatisfactorySave::StructMapTypeList::serializeEntry(Archive& ar, std::size_t i) {
    if (ar.isIArchive()) {
        if (List.size() <= i) {
            List.resize(i + 1);
        }
        List[i] = Struct::create(struct_name_, ar);
    } else {
        ar << *List[i];
    }
}
