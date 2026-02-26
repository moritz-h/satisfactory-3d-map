#pragma once

#include <cstdint>
#include <span>
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
    protected:
        std::span<FPropertyTypeNameNode> Nodes;

    public:
        [[nodiscard]] inline bool IsEmpty() const {
            return Nodes.empty();
        }

        [[nodiscard]] inline const FName& GetName() const {
            return Nodes[0].Name;
        }

        [[nodiscard]] inline FPropertyTypeName GetParameter(int32_t ParamIndex) const {
            const FPropertyTypeNameNode& First = Nodes[0];
            if (ParamIndex < 0 || ParamIndex >= First.InnerCount) {
                return {};
            }
            int32_t ParamIdxPtr = 1;
            for (int32_t Skip = ParamIndex; Skip > 0; Skip--, ParamIdxPtr++) {
                Skip += Nodes[ParamIdxPtr].InnerCount;
            }
            int32_t Remaining = Nodes[ParamIdxPtr].InnerCount;
            int Length = 1;
            while (Remaining > 0) {
                Remaining += Nodes[ParamIdxPtr + Length].InnerCount - 1;
                Length++;
            }
            FPropertyTypeName Param;
            Param.Nodes = Nodes.subspan(ParamIdxPtr, Length);
            return Param;
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

    /**
     * FPropertyTypeName_Data is a custom wrapper to actually hold the FPropertyTypeName data. Unreal internally
     * instead uses a global table, but here data should be held locally.
     */
    class SATISFACTORYSAVE_API FPropertyTypeName_Data : public FPropertyTypeName {
    protected:
        std::vector<FPropertyTypeNameNode> Nodes_Data;

    public:
        FPropertyTypeName_Data() = default;
        FPropertyTypeName_Data(const FPropertyTypeName_Data& other) : Nodes_Data(other.Nodes_Data) {
            Nodes = Nodes_Data;
        }
        FPropertyTypeName_Data& operator=(const FPropertyTypeName_Data& other) {
            if (this != &other) {
                Nodes_Data = other.Nodes_Data;
                Nodes = Nodes_Data;
            }
            return *this;
        }
        FPropertyTypeName_Data(FPropertyTypeName_Data&&) noexcept = default;
        FPropertyTypeName_Data& operator=(FPropertyTypeName_Data&&) noexcept = default;

        void serialize(Archive& ar) {
            if (ar.isIArchive()) {
                Nodes_Data.clear();
                int32_t Remaining = 1;
                do {
                    FPropertyTypeNameNode& Node = Nodes_Data.emplace_back();
                    ar << Node;
                    Remaining += Node.InnerCount - 1;
                } while (Remaining > 0);
                // Update Nodes span.
                Nodes = Nodes_Data;
            } else {
                for (auto& Node : Nodes_Data) {
                    ar << Node;
                }
            }
        }
    };
} // namespace SatisfactorySave
