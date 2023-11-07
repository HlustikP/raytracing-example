#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <expected>

namespace utils {

enum class ReadFileError {
    GENERIC,
    FILE_DOES_NOT_EXIST
};

enum class WriteFileError {
    GENERIC,
    CANNOT_OPEN_FILE_STREAM
};

namespace fs = std::filesystem;

[[nodiscard]] std::expected<std::string, ReadFileError> readFile(const fs::path &path) noexcept;

template<typename T>
std::expected<const fs::path*, WriteFileError> writeFile(const fs::path &path,
                                                          const T *content,
                                                          size_t characters_count) noexcept {
    std::ofstream stream(path, std::ios::out | std::ios::binary);
    if (!stream.is_open()) {
        std::cerr << "[ERROR] [FILE] Unable to create file" << std::endl;
        return std::unexpected(WriteFileError::CANNOT_OPEN_FILE_STREAM);
    }

    try {
        stream.write(reinterpret_cast<const char *>(content), sizeof(T) * characters_count);
    } catch (const std::exception &e) {
        std::cerr << "[ERROR] [FILE] " << e.what() << std::endl;
        return std::unexpected(WriteFileError::GENERIC);
    }

    return &path;
}

int renderImage(int image_width, int image_height, const utils::fs::path& outfile_path);

} // namespace utils
