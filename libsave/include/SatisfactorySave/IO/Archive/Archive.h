#pragma once

#include <concepts>
#include <filesystem>
#include <stdexcept>
#include <type_traits>
#include <vector>

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
            for (auto& val : v) {
                *this << val;
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

    protected:
        Archive() = default;
        ~Archive() = default;

        virtual void serialize(void* data, std::size_t size) = 0;
        virtual void serializeString(std::string& s) = 0;
        virtual void serializeName(FName& n);
        virtual void serializeObjectReference(FObjectReferenceDisc& ref);
        virtual void validateReadLimit(std::size_t){};
    };
} // namespace SatisfactorySave
