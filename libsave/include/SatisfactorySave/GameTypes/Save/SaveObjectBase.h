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
        static std::shared_ptr<SaveObjectBase> create(IStreamArchive& ar);

    protected:
        SaveObjectBase(bool isActor);

    public:
        virtual ~SaveObjectBase() = default;

        virtual void serialize(Archive& ar);

        virtual void serializeProperties(Archive& ar, int32_t length);

        [[nodiscard]] bool isActor() const {
            return isActor_;
        }

        // TODO
        int32_t unk1 = 0;
        int32_t unk2 = 0;

    protected:
        bool isActor_ = false;

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
