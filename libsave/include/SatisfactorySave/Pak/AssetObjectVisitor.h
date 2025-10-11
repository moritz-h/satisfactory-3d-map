#pragma once

#include <typeinfo>

#include "../GameTypes/UE/Engine/Engine/StaticMesh.h"
#include "../GameTypes/UE/Engine/Engine/Texture2D.h"
#include "SatisfactorySave/GameTypes/UE/CoreUObject/UObject/Object.h"
#include "SatisfactorySave/GameTypes/UE/Engine/GameFramework/Actor.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    class SATISFACTORYSAVE_API AssetObjectVisitor {
    public:
        virtual ~AssetObjectVisitor() = default;

        virtual void visit(UObject& o) = 0;
        virtual void visit(AActor& o) = 0;
        virtual void visit(UStaticMesh& o) = 0;
        virtual void visit(UTexture2D& o) = 0;

        void dispatch(UObject& obj) {
            if (typeid(obj) == typeid(UObject)) {
                visit(static_cast<UObject&>(obj));
            } else if (typeid(obj) == typeid(AActor)) {
                visit(static_cast<AActor&>(obj));
            } else if (typeid(obj) == typeid(UStaticMesh)) {
                visit(static_cast<UStaticMesh&>(obj));
            } else if (typeid(obj) == typeid(UTexture2D)) {
                visit(static_cast<UTexture2D&>(obj));
            } else {
                visit(static_cast<UObject&>(obj));
            }
        }
    };
} // namespace SatisfactorySave
