#pragma once

#include <memory>
#include <utility>

#include "SatisfactorySave/Pak/AssetExport.h"
#include "SatisfactorySave/Pak/AssetObjectVisitor.h"

#include "../OpenGL/Texture.h"
#include "CommonUI.h"

namespace s = SatisfactorySave;

namespace Satisfactory3DMap::UI {

    class AssetObjectEditor {
    public:
        AssetObjectEditor(std::shared_ptr<s::AssetExport> assetExport, const EventContext& ctx)
            : assetExport_(std::move(assetExport)),
              ctx_(ctx) {}
        ~AssetObjectEditor() = default;

        void renderGui();

    protected:
        std::shared_ptr<s::AssetExport> assetExport_;
        const EventContext& ctx_;
        std::unique_ptr<OGLTexture2D> texture2d_;

        class AssetUObjectEditor : public s::AssetObjectVisitor {
        protected:
            AssetObjectEditor& parent_;

        public:
            explicit AssetUObjectEditor(AssetObjectEditor& parent) : parent_(parent) {}

            void visit(s::UObject& o) override;
            void visit(s::UStaticMesh& o) override;
            void visit(s::UTexture2D& o) override;
        };
    };
} // namespace Satisfactory3DMap::UI
