#pragma once

#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "../../Utils/StackUtils.h"
#include "../IOStream.h"
#include "Archive.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API IStreamArchive : public Archive {
    public:
        explicit IStreamArchive(std::vector<char>&& buf)
            : data_buf_(std::make_unique<std::vector<char>>(std::move(buf))) {
            istream_ = std::make_unique<MemoryIStream>(std::as_bytes(std::span{data_buf_->data(), data_buf_->size()}));
        }

        explicit IStreamArchive(std::span<const char> buf) {
            istream_ = std::make_unique<MemoryIStream>(std::as_bytes(buf));
        }

        explicit IStreamArchive(const std::filesystem::path& path) {
            istream_ = std::make_unique<FileIStream>(path);
        }

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
            return istream_->tell();
        }

        void seek(std::size_t pos) override {
            istream_->seek(pos);
        }

        [[nodiscard]] std::size_t size() const {
            return istream_->size();
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
        void serialize(void* data, std::size_t size) override;

        void serializeString(std::string& s) override;

        void serializeName(FName& n) override;

        void validateReadLimit(std::size_t size) override;

        std::unique_ptr<std::vector<char>> data_buf_;
        std::unique_ptr<IStream> istream_;
        std::stack<std::size_t> read_limits_;
        std::stack<std::string> parent_class_info_;
    };
} // namespace SatisfactorySave
