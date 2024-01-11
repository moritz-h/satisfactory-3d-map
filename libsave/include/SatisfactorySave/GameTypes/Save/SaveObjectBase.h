#pragma once

#include <cstdint>
#include <istream>
#include <memory>
#include <ostream>
#include <string>

#include "../../IO/Archive/IStreamArchive.h"
#include "../Properties/Base/PropertyList.h"
#include "../UE/Satisfactory/ObjectReference.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    // FObjectBaseSaveHeader
    class SATISFACTORYSAVE_API SaveObjectBase {
    public:
        static std::shared_ptr<SaveObjectBase> create(int32_t globalId, int32_t levelId, IStreamArchive& ar);

        SaveObjectBase(int32_t globalId, int32_t levelId);
        virtual ~SaveObjectBase() = default;

        virtual void serialize(Archive& ar);

        virtual void serializeProperties(Archive& ar, int32_t length);

        [[nodiscard]] int32_t globalId() const {
            return globalId_;
        }

        [[nodiscard]] int32_t levelId() const {
            return levelId_;
        }

        [[nodiscard]] int32_t type() const {
            return type_;
        }

        // TODO
        int32_t unk1 = 0;
        int32_t unk2 = 0;

    protected:
        int32_t globalId_ = 0;
        int32_t levelId_ = 0;
        int32_t type_ = 0;

    public:
        // FObjectBaseSaveHeader
        std::string ClassName;
        FObjectReferenceDisc Reference;

        PropertyList Properties;
        bool HasGuid = false;
        FGuid Guid;
        std::vector<char> ExtraProperties;
    };
} // namespace SatisfactorySave
