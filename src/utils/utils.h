#pragma once

#include <cassert>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <expected>
#include <random>
#include <utility>
#include <optional>

namespace utils {

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr auto pi = 3.1415926535897932385;
inline double degrees_to_radians(const double degrees) {
    return degrees * pi / 180.0;
}

/**
 * \brief Error codes for readFile
 */
enum class ReadFileError {
    GENERIC,
    FILE_DOES_NOT_EXIST
};

/**
 * \brief Error codes for writeFile
 */
enum class WriteFileError {
    GENERIC,
    CANNOT_OPEN_FILE_STREAM
};

namespace fs = std::filesystem;

/**
 * \brief Read a file from the filesystem
 * \param path the path to the file
 * \return the file contents on success, or an error code on failure
 */
[[nodiscard]] std::expected<std::string, ReadFileError> readFile(const fs::path &path) noexcept;

/**
 * \brief Write a file to the filesystem
 * \tparam T the type of the content
 * \param path the path to the file
 * \param content a pointer to the content
 * \param characters_count the number of characters to write
 * \return the path to the file on success, or an error code on failure
 */
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

/**
 * \return a random real in the range [0, 1]
 */
inline double getRandomNormalDouble() {
    return distribution(generator);
}

/**
 * \param min the minimum value
 * \param max the maximum value
 * \return a random real in the inclusive range [min, max]
 */
inline double getRandomDouble(const double min, const double max) {
    assert(min < max);
    return min + (max - min) * getRandomNormalDouble();
}

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

/**
 * \brief Helper Class to handle user input.
 * \tparam T the type to convert the input to
 */
template <typename T>
class InputHelper {
public:
    explicit InputHelper(std::string  question, const T& defaultValue = T{})
        : question(std::move(question)), defaultValue(defaultValue) {}

    /**
     * \return the user input converted to type T, or std::nullopt if the input could not be converted
     */
    std::optional<T> get() const {
        std::cout << question << " (Press Enter for Default: " << defaultValue << "): ";

        std::string userInput;
        std::getline(std::cin, userInput);

        if (userInput.empty()) {
            return defaultValue;
        }

        try {
            T result = parseInput(userInput);
            return result;
        } catch (...) {
            // Error parsing input
            return std::nullopt;
        }
    }

private:
    std::string question;
    T defaultValue;

    /**
     * \brief Parses the user input to type T.
     * Supported types are int, double, std::string, and const char*
     * \param userInput the user input
     * \return the user input converted to type T on success. Throws a compile time error if T is not supported.
     */
    T parseInput(const std::string& userInput) const {
        if constexpr (std::is_same_v<T, int>) {
            return std::stoi(userInput);
        } else if constexpr (std::is_same_v<T, double>) {
            return std::stod(userInput);
        } else if constexpr (std::is_same_v<T, std::string>) {
            return userInput;
        } else if constexpr (std::is_same_v<T, const char*>) {
            return userInput.c_str();
        } else {
            static_assert(std::is_same_v<T, int> || std::is_same_v<T, double> || std::is_same_v<T, std::string> ||
                          std::is_same_v<T, const char*>, "Unsupported type. Only int, double, std::string, and const char* are supported.");
            return;
        }
    }
};

} // namespace utils
