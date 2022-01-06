#ifndef SATISFACTORY3DMAP_OFSTREAMARCHIVE_H
#define SATISFACTORY3DMAP_OFSTREAMARCHIVE_H

#include <fstream>

#include "Archive.h"

namespace Satisfactory3DMap {
    class OFStreamArchive : public Archive {
    public:
        explicit OFStreamArchive(const std::filesystem::path& filepath) {
            file_ = std::ofstream(filepath, std::ios::binary);
            if (!file_.is_open()) {
                throw std::runtime_error("Cannot write file!");
            }
        }

        std::ofstream& rawStream() {
            return file_;
        };

    protected:
        void serialize(void* data, std::size_t size) override {
            file_.write(reinterpret_cast<char*>(data), static_cast<std::streamsize>(size));
            if (!file_.good()) {
                throw std::runtime_error("Error writing to stream!");
            }
        }

        void serializeString(std::string& s) override {
            write_length_string(file_, s);
        }

    private:
        std::ofstream file_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_OFSTREAMARCHIVE_H
