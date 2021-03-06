#pragma once

#include <cstdint>
#include <istream>
#include <memory>
#include <ostream>
#include <string>

#include "GameTypes/ObjectReference.h"
#include "GameTypes/Properties/Properties.h"
#include "IO/Archive/IStreamArchive.h"

namespace Satisfactory3DMap {

    // FObjectBaseSaveHeader
    class SaveObjectBase {
    public:
        static std::shared_ptr<SaveObjectBase> create(int32_t id, IStreamArchive& ar);

        explicit SaveObjectBase(int32_t id);
        virtual ~SaveObjectBase() = default;

        virtual void serialize(Archive& ar);

        virtual void serializeProperties(Archive& ar, int32_t length);

        [[nodiscard]] int32_t id() const {
            return id_;
        }

        [[nodiscard]] int32_t type() const {
            return type_;
        }

        [[nodiscard]] const std::string& className() const {
            return class_name_;
        }

        [[nodiscard]] const ObjectReference& reference() const {
            return reference_;
        }

        [[nodiscard]] const Properties& properties() const {
            return properties_;
        }

        [[nodiscard]] const std::vector<char>& extraProperties() const {
            return extraProperties_;
        }

    protected:
        int32_t id_ = 0;
        int32_t type_ = 0;
        std::string class_name_;
        ObjectReference reference_;

        Properties properties_;
        bool hasGuid_ = false;
        FGuid guid_;
        std::vector<char> extraProperties_;
    };
} // namespace Satisfactory3DMap
