#ifndef SATISFACTORY3DMAP_IFSTREAMARCHIVE_H
#define SATISFACTORY3DMAP_IFSTREAMARCHIVE_H

#include <fstream>

#include "Archive.h"

namespace Satisfactory3DMap {
    class IFStreamArchive : public Archive {
    public:
        explicit IFStreamArchive(const std::filesystem::path& filepath) {
            file_ = std::ifstream(filepath, std::ios::binary);
            if (!file_.is_open()) {
                throw std::runtime_error("Cannot read file!");
            }

            // File size
            file_.seekg(0, std::ios::end);
            filesize_ = file_.tellg();
            file_.seekg(0, std::ios::beg);
        }

        std::ifstream& rawStream() {
            return file_;
        };

        std::size_t size() const {
            return filesize_;
        }

    protected:
        void serialize(void* data, std::size_t size) override {
            file_.read(reinterpret_cast<char*>(data), static_cast<std::streamsize>(size));
            if (!file_.good()) {
                throw std::runtime_error("Error reading from stream!");
            }
        }

        void serializeString(std::string& s) override {
            s = read_length_string(file_);
        }

    private:
        std::ifstream file_;
        std::size_t filesize_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_IFSTREAMARCHIVE_H
