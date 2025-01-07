#pragma once

#include "SatisfactorySave/GameTypes/Save/ObjectVisitor.h"
#include "SatisfactorySave/GameTypes/UE/CoreUObject/UObject/Object.h"

#include "CommonUI.h"
#include "MapWindow/DataView/ObjectProxy.h"

namespace s = SatisfactorySave;

namespace Satisfactory3DMap::UI {

    class ObjectEditor {
    public:
        explicit ObjectEditor(const EventContext& ctx = {}) : ctx_(ctx) {};
        ~ObjectEditor() = default;

        void renderGui(ObjectProxyPtr proxy) const;
        void renderGui(s::UObject& obj, int32_t object_id) const;

    protected:
        const EventContext& ctx_;

        class UObjectEditor : public s::ObjectVisitor {
        protected:
            const ObjectEditor& parent_;
            const int32_t object_id_;

        public:
            explicit UObjectEditor(const ObjectEditor& parent, int32_t object_id)
                : parent_(parent),
                  object_id_(object_id) {}

            void visit(s::UObject& o) override;
            void visit(s::AActor& o) override;
            void visit(s::AFGBuildableConveyorBase& o) override;
            void visit(s::AFGConveyorChainActor& o) override;
            void visit(s::AFGBuildableWire& o) override;
            void visit(s::AFGCircuitSubsystem& o) override;
            void visit(s::AFGLightweightBuildableSubsystem& o) override;
            void visit(s::AFGGameMode& o) override;
            void visit(s::AFGGameState& o) override;
            void visit(s::AFGPlayerState& o) override;
            void visit(s::AFGVehicle& o) override;
            void visit(s::AFGRailroadVehicle& o) override;
            void visit(s::AFGDroneVehicle& o) override;
            void visit(s::UActorComponent& o) override;
        };
    };
} // namespace Satisfactory3DMap::UI
