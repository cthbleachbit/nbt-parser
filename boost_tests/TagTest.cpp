#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include "libnbtp.h"
#include <iostream>

BOOST_AUTO_TEST_SUITE(TagTester)

	BOOST_AUTO_TEST_CASE(AssignName) {
		NBTP::CompoundTag testTag;
		auto _tag = new NBTP::ByteTag(3);
		auto _tag2 = new NBTP::IntTag(4);
		auto _tag3 = new NBTP::LongTag(21242343434L);
		testTag.insert("sometag", std::shared_ptr<NBTP::Tag>(_tag));
		testTag.insert("sometag2", std::shared_ptr<NBTP::Tag>(_tag2));
		testTag.insert("sometag3", std::shared_ptr<NBTP::Tag>(_tag3));
		std::cout << testTag;
		BOOST_ASSERT(testTag.size() == (ssize_t) 3);
	}

	BOOST_AUTO_TEST_CASE(AssignReplace) {
		NBTP::CompoundTag testTag;
		auto _tag = new NBTP::ByteTag(3);
		auto _tag2 = new NBTP::IntTag(5);
		testTag.insert("sometag", std::shared_ptr<NBTP::Tag>(_tag));
		testTag.insert("sometag", std::shared_ptr<NBTP::Tag>(_tag2));
		testTag.textOutput(std::cout, 0);
		BOOST_ASSERT(testTag.size() == (ssize_t) 1);
	}

	BOOST_AUTO_TEST_CASE(ListAdd) {
		NBTP::ListTag testTag;
		testTag.setContentType(NBTP::TagType::INT);
		testTag.setContentType(NBTP::TagType::COMPOUND);
		auto _tag = new NBTP::ByteTag(3);
		auto _tag2 = new NBTP::ByteTag(5);
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

	BOOST_AUTO_TEST_CASE(TagCompare) {
		auto L1 = NBTP::LongTag(3);
		auto R1 = NBTP::ShortTag(5);
		BOOST_ASSERT(L1 != R1);
		auto L2 = NBTP::FloatTag(0.5f);
		auto R2 = NBTP::FloatTag(0.4f);
		BOOST_ASSERT(L2 != R2);
		auto L3 = NBTP::DoubleTag(1.4);
		auto R3 = NBTP::DoubleTag(1.4);
		BOOST_ASSERT(L3 == R3);

		auto PL1 = std::shared_ptr<NBTP::Tag>(new NBTP::ByteTag(3));
		auto PR1 = std::shared_ptr<NBTP::ByteTag>(new NBTP::ByteTag(5));
		BOOST_ASSERT(*PL1 != *PR1);
		auto PL2 = std::shared_ptr<NBTP::Tag>(new NBTP::IntTag(3));
		auto PR2 = std::shared_ptr<NBTP::Tag>(new NBTP::IntTag(3));
		BOOST_ASSERT(*PL2 == *PR2);
	}

	BOOST_AUTO_TEST_CASE(SingleValuedTagCopyMove) {
		// MOVE ASSIGNMENT
		NBTP::LongTag L1 = NBTP::LongTag(3);
		// COPY ASSIGNMENT
		NBTP::LongTag L2 = L1;
		L2.setPayload(20L);
		BOOST_ASSERT(L1.getPayload() == 3L);
		BOOST_ASSERT(L2.getPayload() == 20L);
	}

	BOOST_AUTO_TEST_CASE(CompoundCopyMove) {
		NBTP::CompoundTag T1;
		T1.insert("S1", std::make_shared<NBTP::IntTag>(10));
		BOOST_ASSERT(T1.size() == 1);
		// COPY ASSIGNMENT
		NBTP::CompoundTag T2 = T1;
		BOOST_ASSERT(T1.size() == 1);
		T2.insert("S2", std::make_shared<NBTP::StringTag>("tag value"));
		BOOST_ASSERT(T1.size() == 1);
		NBTP::Tag *inttag = T2.lookup("S1").get();
		((NBTP::IntTag* ) inttag)->setPayload(50);
		std::cout << T1 << std::endl;
		std::cout << T2 << std::endl;
	}

	BOOST_AUTO_TEST_CASE(ListCopyMove) {
		using namespace NBTP;
		ListTag L1 = ListTag(TagType::STRING);
		ListTag L2(TagType::INT);
		L1.insert(std::make_shared<StringTag>("blah"));
		L2.insert(std::make_shared<IntTag>(50));
		L2.insert(std::make_shared<IntTag>(100));
		L1 = L2;
		std::cout << L1 << std::endl;
		std::cout << L2 << std::endl;
		BOOST_ASSERT(L1.size() == 2);
		BOOST_ASSERT(L2.size() == 2);
		L2.insert(std::make_shared<IntTag>(250));
		BOOST_ASSERT(L1.size() == 2);
		BOOST_ASSERT(L2.size() == 3);
	}

BOOST_AUTO_TEST_SUITE_END()