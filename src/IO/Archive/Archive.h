#ifndef SATISFACTORY3DMAP_ARCHIVE_H
#define SATISFACTORY3DMAP_ARCHIVE_H

#include <filesystem>
#include <stdexcept>
#include <type_traits>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Satisfactory3DMap {
    class Archive;

    template<class T>
    struct dependent_false : std::false_type {};

    template<typename C>
    struct has_serialize {
    private:
        template<typename T>
        static constexpr auto check(T*) ->
            typename std::is_same<decltype(std::declval<T>().serialize(std::declval<Archive&>())), void>::type;

        template<typename>
        static constexpr std::false_type check(...);

        typedef decltype(check<C>(nullptr)) type;

    public:
        static constexpr bool value = type::value;
    };

    class Archive {
    public:
        template<typename T>
        inline Archive& operator<<(T& v) {
            if constexpr (std::is_arithmetic_v<T> || std::is_same_v<glm::vec3, T> || std::is_same_v<glm::quat, T>) {
                serialize(&v, sizeof(T));
            } else if constexpr (has_serialize<T>::value) {
                v.serialize(*this);
            } else {
                static_assert(dependent_false<T>::value, "No serialization available!");
            }
            return *this;
        }

        inline Archive& operator<<(bool& v) {
            int32_t tmp = v ? 1 : 0;
            serialize(&tmp, sizeof(int32_t));
            v = !!tmp;
            return *this;
        }

        inline Archive& operator<<(std::string& s) {
            serializeString(s);
            return *this;
        }

        virtual bool isIArchive() const = 0;
        virtual bool isOArchive() const = 0;

        virtual std::size_t tell() = 0;
        virtual void seek(std::size_t pos) = 0;

    protected:
        Archive() = default;
        ~Archive() = default;

        virtual void serialize(void* data, std::size_t size) = 0;
        virtual void serializeString(std::string& s) = 0;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_ARCHIVE_H
