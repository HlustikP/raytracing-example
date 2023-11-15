#include <gtest/gtest.h>
#include "../src/utils/utils.h"

class CleanupFixture : public ::testing::Test {
protected:
    void TearDown() override {
        std::filesystem::remove("test_output.txt");
    }
};

TEST(UtilsTest, WriteFile) {
    std::filesystem::path outputPath("test_output.txt");

    // Your test data
    constexpr auto testData = "Hello\n this is a test message.";

    const auto result = utils::writeFile(outputPath, testData, strlen(testData));

    ASSERT_TRUE(result.value()); // Check if the write was successful
    ASSERT_EQ(*(result.value()), outputPath); // Check if the returned path matches the expected output path
}

TEST(UtilsTest, ReadFile) {
    // Replace "path/to/your/test/file.txt" with an existing file path for testing
    std::filesystem::path filePath("test_input.txt");

    const auto result = utils::readFile(filePath);

    ASSERT_TRUE(result.has_value()); // Check if the read was successful
    ASSERT_FALSE(result.value().empty()); // Check if the content is not empty
}

TEST(UtilsTest, RandomDouble) {
    const double randomValue = utils::getRandomDouble(0.0, 1.0);

    ASSERT_GE(randomValue, 0.0); // Check if the random value is greater than or equal to 0.0
    ASSERT_LE(randomValue, 1.0); // Check if the random value is less than or equal to 1.0
}

TEST_F(CleanupFixture, Cleanup) {
    // This test is only here to clean up the file created by the WriteFile test
    ASSERT_TRUE(true);
}
