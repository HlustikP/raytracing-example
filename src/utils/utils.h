#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <expected>
#include <random>

namespace utils {

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr auto pi = 3.1415926535897932385;
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

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

inline static std::uniform_real_distribution distribution(0.0, 1.0);
inline static std::mt19937 generator { static_cast<std::mt19937::result_type>(
    std::chrono::steady_clock::now().time_since_epoch().count()
) };

// Returns a random real in the range [0, 1]
inline double getRandomNormalDouble() {
    return distribution(generator);
}

int renderImage(int image_width, double aspect_ratio, const fs::path &outfile_path);

// Overload std::cout to print ReadFileError
inline std::ostream &operator<<(std::ostream &os, const ReadFileError &error) {
    switch (error) {
        case ReadFileError::GENERIC:
            os << "GENERIC (Code 0)";
            break;
        case ReadFileError::FILE_DOES_NOT_EXIST:
            os << "FILE_DOES_NOT_EXIST (Code 1)";
            break;
        default:
            os << "UNKNOWN_ERROR";
    }
    return os;
}

// Overload std::cout to print WriteFileError
inline std::ostream &operator<<(std::ostream &os, const WriteFileError &error) {
    switch (error) {
        case WriteFileError::GENERIC:
            os << "GENERIC (Code 0)";
            break;
        case WriteFileError::CANNOT_OPEN_FILE_STREAM:
            os << "CANNOT_OPEN_FILE_STREAM (Code 1)";
            break;
        default:
            os << "UNKNOWN_ERROR";
    }
    return os;
}

} // namespace utils
