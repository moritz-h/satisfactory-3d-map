#pragma once

#include <fstream>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "../../Utils/StackUtils.h"
#include "Archive.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API IStreamArchive : public Archive {
    public:
        explicit IStreamArchive(std::unique_ptr<std::istream> istream) : istream_(std::move(istream)) {}

        template<typename T>
        inline T read() {
            T value;
            operator<<(value);
            return value;
        }

        template<typename T>
        inline T read_ahead() {
            const auto pos = tell();
            T value;
            operator<<(value);
            seek(pos);
            return value;
        }

        template<typename T>
        inline void read_assert_zero() {
            if (read<T>() != static_cast<T>(0L)) {
                throw std::runtime_error("Values from stream is not zero!");
            }
        }

        inline std::vector<char> read_buffer(std::size_t size) {
            std::vector<char> vec(size);
            serialize(vec.data(), size * sizeof(char));
            return vec;
        }

        [[nodiscard]] bool isIArchive() const override {
            return true;
        }

        [[nodiscard]] bool isOArchive() const override {
            return false;
        }

        std::size_t tell() override {
            return static_cast<std::size_t>(istream_->tellg());
        }

        void seek(std::size_t pos) override {
            istream_->seekg(static_cast<std::istream::pos_type>(pos));
        }

        std::istream& rawStream() {
            return *istream_;
        }

        inline auto pushReadLimit(std::size_t size) {
            if (!read_limits_.empty() && size > read_limits_.top()) {
                throw std::runtime_error("Increasing read limit not allowed!");
            }
            return make_stack_guard_push(read_limits_, size);
        }

        inline auto pushParentClassInfo(auto name) {
            return make_stack_guard_push(parent_class_info_, name);
        }

        inline const std::string& getParentClassInfo() {
            static const std::string empty;
            return !parent_class_info_.empty() ? parent_class_info_.top() : empty;
        }

    protected:
        IStreamArchive() = default;

        void serialize(void* data, std::size_t size) override;

        void serializeString(std::string& s) override;

        void serializeName(FName& n) override;

        void validateReadLimit(std::size_t size) override;

        std::unique_ptr<std::istream> istream_;
        std::stack<std::size_t> read_limits_;
        std::stack<std::string> parent_class_info_;
    };

    class SATISFACTORYSAVE_API IFStreamArchive : public IStreamArchive {
    public:
        explicit IFStreamArchive(const std::filesystem::path& filepath) {
            auto file = std::make_unique<std::ifstream>(filepath, std::ios::binary);
            if (!file->is_open()) {
                throw std::runtime_error("Cannot read file: " + filepath.string());
            }

            // File size
            file->seekg(0, std::ios::end);
            filesize_ = file->tellg();
            file->seekg(0, std::ios::beg);

            istream_ = std::move(file);
        }

        [[nodiscard]] std::size_t size() const {
            return filesize_;
        }

    protected:
        std::size_t filesize_ = 0;
    };
} // namespace SatisfactorySave
