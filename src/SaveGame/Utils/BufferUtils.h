#ifndef SATISFACTORYSAVEGAME_BUFFERUTILS_H
#define SATISFACTORYSAVEGAME_BUFFERUTILS_H

#include <streambuf>
#include <vector>

namespace SatisfactorySaveGame {
    class charvectorbuf : public std::streambuf {
    public:
        explicit charvectorbuf(std::vector<char>& vec) {
            setg(vec.data(), vec.data(), vec.data() + vec.size());
        }

        // https://stackoverflow.com/a/53200040
        std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which) override {
            return gptr() - eback();
        }
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_BUFFERUTILS_H
