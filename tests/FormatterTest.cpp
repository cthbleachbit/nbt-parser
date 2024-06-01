#include <gtest/gtest.h>
#include <fmt/format.h>
#include "libnbtp.h"

TEST(TypeFormattableTest, ValidTagType) {
    NBTP::TagType test_type = NBTP::TagType::INTS;
    std::string tag_formatted = fmt::format("{}", test_type);
    std::string int_formatted = fmt::format("{}", static_cast<int>(test_type));
    EXPECT_EQ(std::string(int_formatted), tag_formatted);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
