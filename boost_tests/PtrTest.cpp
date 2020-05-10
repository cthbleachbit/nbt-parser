#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "libnbtp.h"
#include <iostream>

#include "../include/Tag.h"

BOOST_AUTO_TEST_SUITE(PtrTester)

	BOOST_AUTO_TEST_CASE(SmartPtr) {
		std::shared_ptr<NBTP::Tag> _tag = std::shared_ptr<NBTP::ByteTag>(new NBTP::ByteTag(3));
		auto _copy = _tag;
		BOOST_ASSERT(_copy->typeCode() == NBTP::TagType::BYTE);
	}

BOOST_AUTO_TEST_SUITE_END()