#pragma once

#include <concepts>
#include <filesystem>
#include <limits>
#include <optional>
#include <stack>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include "../../Utils/StackUtils.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class Archive;
    class FName;
    struct FObjectReferenceDisc;

    template<class T>
    struct dependent_false : std::false_type {};

    template<typename T>
    concept HasSerialize = requires(T& t, Archive& ar) {
        { t.serialize(ar) } -> std::same_as<void>;
    };

    template<typename T>
    concept IsSerializableObject = std::is_arithmetic_v<T> || HasSerialize<T> || std::derived_from<T, std::string> ||
                                   std::derived_from<T, FName> || std::derived_from<T, FObjectReferenceDisc>;

    // Use a trait to model the concept IsSerializableObject or arbitrary nested vector of IsSerializableObject.
    template<typename T>
    struct IsSerializableTrait : std::false_type {};

    template<IsSerializableObject T>
    struct IsSerializableTrait<T> : std::true_type {};

    template<typename T>
    struct IsSerializableTrait<std::vector<T>> : IsSerializableTrait<T> {};

    template<typename T>
    concept IsSerializable = IsSerializableTrait<T>::value;

    class SATISFACTORYSAVE_API Archive {
    public:
        template<typename T>
        inline Archive& operator<<(T& v) {
            if constexpr (std::is_arithmetic_v<T>) {
                serialize(&v, sizeof(T));
            } else if constexpr (HasSerialize<T>) {
                v.serialize(*this);
            } else {
                static_assert(dependent_false<T>::value, "No serialization available!");
            }
            return *this;
        }

        // TArray<T>
        template<typename T>
        inline Archive& operator<<(std::vector<T>& v) {
            int32_t Num = static_cast<int32_t>(v.size());
            *this << Num;
            validateReadLimit(static_cast<std::size_t>(Num));
            v.resize(Num);
            if constexpr (std::is_arithmetic_v<T>) {
                serialize(v.data(), Num * sizeof(T));
            } else {
                for (auto& val : v) {
                    *this << val;
                }
            }
            return *this;
        }

        template<typename T>
        inline Archive& operator<<(std::optional<T>& v) {
            bool hasValue = v.has_value();
            *this << hasValue;
            if (hasValue) {
                if (isIArchive()) {
                    v.emplace();
                }
                *this << v.value();
            }
            return *this;
        }

        inline Archive& operator<<(bool& v) {
            uint32_t tmp = v ? 1 : 0;
            serialize(&tmp, sizeof(uint32_t));
            v = !!tmp;
            return *this;
        }

        inline Archive& operator<<(std::string& s) {
            serializeString(s);
            return *this;
        }

        inline Archive& operator<<(FName& n) {
            serializeName(n);
            return *this;
        }

        inline Archive& operator<<(FObjectReferenceDisc& ref) {
            serializeObjectReference(ref);
            return *this;
        }

        inline void serializeRaw(void* data, std::size_t size) {
            serialize(data, size);
        }

        [[nodiscard]] virtual bool isIArchive() const = 0;
        [[nodiscard]] virtual bool isOArchive() const = 0;

        virtual std::size_t tell() = 0;
        virtual void seek(std::size_t pos) = 0;

        inline auto pushSaveVersion(auto saveVersion) {
            return make_stack_guard_push(save_version_stack_, saveVersion);
        }

        inline int32_t getSaveVersion() {
            // Empty version stack should assume "newest" version.
            return !save_version_stack_.empty() ? save_version_stack_.top() : std::numeric_limits<int32_t>::max();
        }

        inline auto pushLightweightVersion(auto lightweightVersion) {
            return make_stack_guard_push(lightweight_version_stack_, lightweightVersion);
        }

        inline int32_t getLightweightVersion() {
            // Empty version stack should assume "newest" version.
            return !lightweight_version_stack_.empty() ? lightweight_version_stack_.top()
                                                       : std::numeric_limits<int32_t>::max();
        }

    protected:
        Archive() = default;
        virtual ~Archive() = default;

        virtual void serialize(void* data, std::size_t size) = 0;
        virtual void serializeString(std::string& s) = 0;
        virtual void serializeName(FName& n) = 0;
        virtual void serializeObjectReference(FObjectReferenceDisc& ref);
        virtual void validateReadLimit(std::size_t) {}

        std::stack<int32_t> save_version_stack_;
        std::stack<int32_t> lightweight_version_stack_;
    };
} // namespace SatisfactorySave
