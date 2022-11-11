#include <gtest/gtest.h>
#include <cstdint>
#include "tags/SingleValuedTag.h"

template<typename T, std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>, bool> = true>
class SingleValuedTagTest : public ::testing::Test {
protected:
	/**
	 * Test constructors and parsers
	 * @param value value to construct
	 */
	static void testConstruct(T value) {
		using namespace NBTP;
		SingleValuedTag<T> direct(value);
		ASSERT_EQ(direct.getPayload(), value);

		SingleValuedTag<T> copied(direct);
		ASSERT_EQ(direct.getPayload(), copied.getPayload());

		copied.setPayload(value + 1);
		ASSERT_EQ(direct.getPayload() + 1, copied.getPayload());
	}

	static void testParseBinary(T value) {
		using namespace NBTP;

		/* Dump big endian into memory */
		T bigEndian = Conversion::toJ(value);
		std::stringstream ss;
		ss.write(reinterpret_cast<const char *>(&bigEndian), sizeof(T));

		ssize_t counter = 0;
		T parsed = SingleValuedTag<T>::parseBinaryNumeric(ss, counter);
		ASSERT_EQ(counter, sizeof(T));
		ASSERT_EQ(parsed, value);
	}
};

using ValidPrimitives = ::testing::Types<int8_t, int16_t, int32_t, int64_t, float, double>;
TYPED_TEST_SUITE(SingleValuedTagTest, ValidPrimitives);

TYPED_TEST(SingleValuedTagTest, Construct) {
	TestFixture::testConstruct((TypeParam)5);
}

TYPED_TEST(SingleValuedTagTest, ParseBinary) {
	TestFixture::testParseBinary((TypeParam)5);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}