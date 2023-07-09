#pragma once

#include <cstdint>
#include <istream>
#include <memory>
#include <ostream>
#include <string>

#include "../../IO/Archive/IStreamArchive.h"
#include "../Properties/Properties.h"
#include "ObjectReference.h"

namespace SatisfactorySave {

    // FObjectBaseSaveHeader
    class SaveObjectBase {
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
        int32_t globalId_ = 0;
        int32_t levelId_ = 0;
        int32_t type_ = 0;
        std::string class_name_;
        ObjectReference reference_;

        Properties properties_;
        bool hasGuid_ = false;
        FGuid guid_;
        std::vector<char> extraProperties_;
    };
} // namespace SatisfactorySave
