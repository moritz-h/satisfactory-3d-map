#pragma once

#include <cstdint>
#include <vector>

#include "../../../../IO/Archive/Archive.h"
#include "../../Core/UObject/NameTypes.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    struct SATISFACTORYSAVE_API FPropertyTypeNameNode {
    public:
        FName Name;
        int32_t InnerCount = 0;

        void serialize(Archive& ar) {
            ar << Name;
            ar << InnerCount;
        }
    };

    class SATISFACTORYSAVE_API FPropertyTypeName {
    public:
        std::vector<FPropertyTypeNameNode> Nodes;

        void serialize(Archive& ar) {
            if (ar.isIArchive()) {
                Nodes.clear();
                int32_t Remaining = 1;
                do {
                    FPropertyTypeNameNode& Node = Nodes.emplace_back();
                    ar << Node;
                    Remaining += Node.InnerCount - 1;
                } while (Remaining > 0);
            } else {
                for (auto& Node : Nodes) {
                    ar << Node;
                }
            }
        }

        [[nodiscard]] inline const FName& GetName() const {
            return Nodes[0].Name;
        }

        [[nodiscard]] inline FName GetParameterName(int32_t ParamIndex) const {
            const FPropertyTypeNameNode& First = Nodes[0];
            if (ParamIndex < 0 || ParamIndex >= First.InnerCount) {
                return FName{"None"};
            }
            int32_t ParamIdxPtr = 1;
            for (int32_t Skip = ParamIndex; Skip > 0; Skip--, ParamIdxPtr++) {
                Skip += Nodes[ParamIdxPtr].InnerCount;
            }
            return Nodes[ParamIdxPtr].Name;
        }
    };
} // namespace SatisfactorySave
