#include "OodleUtils.h"

#include <cstdint>
#include <stdexcept>

#ifdef _WIN32
#include <Windows.h>

namespace {
    typedef intptr_t(__stdcall* OodleLZ_DecompressFunc)(const void* compBuf, intptr_t compBufSize, void* rawBuf,
        intptr_t rawLen, int32_t fuzzSafe, int32_t checkCRC, int32_t verbosity, void* decBufBase, intptr_t decBufSize,
        void* fpCallback, void* callbackUserData, void* decoderMemory, intptr_t decoderMemorySize, int32_t threadPhase);

    class OodleDllDispatch {
    private:
        HMODULE handle_ = nullptr;

    public:
        OodleLZ_DecompressFunc OodleLZ_Decompress = nullptr;

        OodleDllDispatch() {
            handle_ = LoadLibrary(TEXT("oo2core_9_win64.dll"));
            if (handle_ == NULL) {
                throw std::runtime_error("Error loading Oodle DLL!");
            }

            OodleLZ_Decompress = (OodleLZ_DecompressFunc) GetProcAddress(handle_, "OodleLZ_Decompress");
            if (OodleLZ_Decompress == NULL) {
                throw std::runtime_error("Error loading OodleLZ_Decompress!");
            }
        }

        ~OodleDllDispatch() {
            if (handle_ != nullptr) {
                FreeLibrary(handle_);
                handle_ = nullptr;
            }
        }

        OodleDllDispatch(const OodleDllDispatch&) = delete;
        OodleDllDispatch& operator=(const OodleDllDispatch&) = delete;
    };

    OodleDllDispatch& dispatch() {
        static OodleDllDispatch d;
        return d;
    }
} // namespace
#endif

void SatisfactorySave::checkOodleLibraryIsLoaded() {
#ifdef _WIN32
    dispatch();
#else
    throw std::runtime_error("Not implemented!");
#endif
}

void SatisfactorySave::oodleDecompress(std::span<std::byte> dest, std::span<const std::byte> source) {
#ifdef _WIN32
    auto len = dispatch().OodleLZ_Decompress(source.data(), static_cast<int64_t>(source.size()), dest.data(),
        static_cast<int64_t>(dest.size()), 1, 0, 0, nullptr, 0, nullptr, nullptr, nullptr, 0, 3);
    if (len != dest.size()) {
        throw std::runtime_error("Error decompressing Oodle!");
    }
#else
    throw std::runtime_error("Not implemented!");
#endif
}
