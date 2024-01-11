#pragma once

#include <istream>
#include <memory>
#include <ostream>
#include <vector>

#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API MemIStreambuf : public std::streambuf {
    private:
        std::unique_ptr<std::vector<char>> buf_;

    public:
        explicit MemIStreambuf(std::unique_ptr<std::vector<char>> buf) : buf_(std::move(buf)) {
            setg(buf_->data(), buf_->data(), buf_->data() + buf_->size());
        }

    protected:
        // https://stackoverflow.com/a/53200040
        // https://stackoverflow.com/a/29134696
        std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way,
            [[maybe_unused]] std::ios_base::openmode which) override {
            // support tellg()
            if (way == std::ios_base::cur && off == 0) {
                return gptr() - eback();
            }
            // TODO
            return -1;
        }

        std::streampos seekpos(std::streampos sp, [[maybe_unused]] std::ios_base::openmode which) override {
            gbump(static_cast<int>(sp) - static_cast<int>(gptr() - eback()));
            return gptr() - eback();
        }
    };

    class SATISFACTORYSAVE_API MemIStream : public std::istream {
    private:
        MemIStreambuf memstreambuf_;

    public:
        explicit MemIStream(std::unique_ptr<std::vector<char>> buf)
            : std::istream(nullptr),
              memstreambuf_(std::move(buf)) {
            init(&memstreambuf_);
        }
    };

    class SATISFACTORYSAVE_API MemOStreambuf : public std::streambuf {
    private:
        std::unique_ptr<std::vector<char>> buf_;

    public:
        explicit MemOStreambuf(std::unique_ptr<std::vector<char>> buf) : buf_(std::move(buf)) {
            setp(buf_->data(), buf_->data() + buf_->size());
        }

        [[nodiscard]] const std::vector<char>& data() const {
            return *buf_;
        }

    protected:
        int_type overflow(int_type c) override {
            // use 1.5x as grow rate, similar to STL
            auto new_size = buf_->size() + buf_->size() / 2;
            // at least use 64 bytes after current write position
            auto offset = pptr() - pbase();

            buf_->resize(std::max(static_cast<std::size_t>(offset) + 64, new_size));

            setp(buf_->data(), buf_->data() + buf_->size());
            pbump(static_cast<int>(offset));
            if (c != EOF) {
                buf_->at(offset) = static_cast<char>(c);
                pbump(1);
            }
            return c;
        }

        std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way,
            [[maybe_unused]] std::ios_base::openmode which) override {
            // support tellp()
            if (way == std::ios_base::cur && off == 0) {
                return pptr() - pbase();
            }
            // TODO
            return -1;
        }

        std::streampos seekpos(std::streampos sp, [[maybe_unused]] std::ios_base::openmode which) override {
            setp(buf_->data(), buf_->data() + buf_->size());
            pbump(static_cast<int>(sp));
            return sp;
        }
    };

    class SATISFACTORYSAVE_API MemOStream : public std::ostream {
    private:
        MemOStreambuf memstreambuf_;

    public:
        MemOStream() : std::ostream(nullptr), memstreambuf_(std::make_unique<std::vector<char>>()) {
            init(&memstreambuf_);
        }

        [[nodiscard]] const std::vector<char>& data() const {
            return memstreambuf_.data();
        }
    };
} // namespace SatisfactorySave
