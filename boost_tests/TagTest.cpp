#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "libnbtp.h"
#include <iostream>

BOOST_AUTO_TEST_SUITE(TagTester)

	BOOST_AUTO_TEST_CASE(AssignName) {
		NBTP::CompoundTag testTag;
		NBTP::ByteTag* _tag = new NBTP::ByteTag(3);
		NBTP::IntTag* _tag2 = new NBTP::IntTag(4);
		testTag.insert("sometag", std::shared_ptr<NBTP::ByteTag>(_tag));
		testTag.insert("sometag2", std::shared_ptr<NBTP::IntTag>(_tag2));
		testTag.textOutput(std::cout, 0);
		BOOST_ASSERT(testTag.size() == (ssize_t) 2);
	}

	BOOST_AUTO_TEST_CASE(AssignReplace) {
		NBTP::CompoundTag testTag;
		NBTP::ByteTag* _tag = new NBTP::ByteTag(3);
		NBTP::IntTag* _tag2 = new NBTP::IntTag(5);
		testTag.insert("sometag", std::shared_ptr<NBTP::Tag>(_tag));
		testTag.insert("sometag", std::shared_ptr<NBTP::Tag>(_tag2));
		testTag.textOutput(std::cout, 0);
		BOOST_ASSERT(testTag.size() == (ssize_t) 1);
	}

	BOOST_AUTO_TEST_CASE(ListAdd) {
		NBTP::ListTag testTag;
		testTag.setContentType(NBTP::TagType::INT);
		testTag.setContentType(NBTP::TagType::COMPOUND);
		NBTP::ByteTag* _tag = new NBTP::ByteTag(3);
		NBTP::ByteTag* _tag2 = new NBTP::ByteTag(5);
		testTag.insert(std::shared_ptr<NBTP::ByteTag>(_tag));
		testTag.insert(std::shared_ptr<NBTP::ByteTag>(_tag2));
		testTag.textOutput(std::cout, 0);
		BOOST_ASSERT(testTag.getContentType() == NBTP::TagType::BYTE);
	}

	BOOST_AUTO_TEST_CASE(ListBadType) {
		NBTP::ListTag testTag;
		auto _tag = new NBTP::ByteTag(3);
		auto _tag2 = new NBTP::IntTag(5);
		testTag.insert(std::shared_ptr<NBTP::Tag>(_tag));
		BOOST_REQUIRE_THROW(testTag.insert(std::shared_ptr<NBTP::Tag>(_tag2)), std::runtime_error);
	}

BOOST_AUTO_TEST_SUITE_END()