#ifndef SATISFACTORYSAVEGAME_VECTORS_H
#define SATISFACTORYSAVEGAME_VECTORS_H

namespace SatisfactorySaveGame {

    class Vec3 {
    public:
        Vec3() : v{} {};
        Vec3(float x, float y, float z) : v{x, y, z} {};

        [[nodiscard]] float x() const {
            return v[0];
        }
        [[nodiscard]] float y() const {
            return v[1];
        }
        [[nodiscard]] float z() const {
            return v[2];
        }

    private:
        float v[3];
    };

    class Vec4 {
    public:
        Vec4() : v{} {};
        Vec4(float x, float y, float z, float w) : v{x, y, z, w} {};
        [[nodiscard]] float x() const {
            return v[0];
        }
        [[nodiscard]] float y() const {
            return v[1];
        }
        [[nodiscard]] float z() const {
            return v[2];
        }
        [[nodiscard]] float w() const {
            return v[3];
        }

    private:
        float v[4];
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_VECTORS_H
