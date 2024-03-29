#include <gtest/gtest.h>
#include <cstdint>
#include "tags/SingleValuedTag.h"

template<typename V, std::enable_if_t<std::is_integral_v<V> || std::is_floating_point_v<V>, bool> = true>
class SingleValuedTagTest : public ::testing::Test {
protected:
	/**
	 * Test constructors and parsers
	 * @param value value to construct
	 */
	static void testConstruct(V value) {
		using namespace NBTP;
		SingleValuedTag<V, TagType::END> direct(value);
		ASSERT_EQ(direct.getPayload(), value);

		SingleValuedTag<V, TagType::END> copied(direct);
		ASSERT_EQ(direct.getPayload(), copied.getPayload());

		copied.setPayload(value + 1);
		ASSERT_EQ(direct.getPayload() + 1, copied.getPayload());
	}

	static void testParseBinary(V value) {
		using namespace NBTP;

		/* Dump big endian into memory */
		V bigEndian = Conversion::toJ(value);
		std::stringstream ss;
		ss.write(reinterpret_cast<const char *>(&bigEndian), sizeof(V));

		ssize_t counter = 0;
		V parsed = SingleValuedTag<V, TagType::END>::parseBinaryNumeric(ss, counter);
		ASSERT_EQ(counter, sizeof(V));
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