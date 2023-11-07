#include "utils.h"

namespace utils {

std::expected<std::string, ReadFileError> readFile(const fs::path &path) noexcept {
    // Check if file exists
    if (!fs::exists(path)) {
        std::cerr << "[ERROR] [FILE] " << path << " does not exist" << std::endl;
        return std::unexpected(ReadFileError::FILE_DOES_NOT_EXIST);
    }

    try {
        std::ifstream stream(path, std::ios::in | std::ios::binary);
        const auto size = fs::file_size(path);
        std::string result(size, '\0');
        stream.read(result.data(), static_cast<std::streamsize>(size));

        return result;
    } catch (const std::exception &e) {
        std::cerr << "[ERROR] [FILE] " << e.what() << std::endl;
        return std::unexpected(ReadFileError::GENERIC);
    }
}

} // namespace utils
