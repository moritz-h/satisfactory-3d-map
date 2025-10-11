#include "Pak/AssetExport.h"

#include "GameTypes/UE/Engine/Engine/StaticMesh.h"
#include "GameTypes/UE/Engine/Engine/Texture2D.h"
#include "GameTypes/UE/Engine/GameFramework/Actor.h"

std::shared_ptr<SatisfactorySave::UObject> SatisfactorySave::AssetUObjectFactory(const std::string& class_name) {
    if (class_name == "/Script/Engine/StaticMesh") {
        return std::make_shared<UStaticMesh>();
    } else if (class_name == "/Script/Engine/Texture2D") {
        return std::make_shared<UTexture2D>();
    } else if (class_name == "/Script/Engine/WorldPartitionHLOD") {
        return std::make_shared<AActor>();
    } else {
        return std::make_shared<UObject>();
    }
}
