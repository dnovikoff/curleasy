#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>

#include <boost/test/unit_test.hpp>

#include <curleasy/buffer.hpp>
using namespace CurlEasy;

class BufferHelper {
public:
	Buffer buffer;

	BufferHelper() {}
	explicit BufferHelper(size_t initialSize):buffer(initialSize) {}
	void append(const std::string& s) {
		buffer.tryAppendData(s.size(), const_cast<char*>(s.c_str()));
	}
	const std::string asString() const {
		return std::string(static_cast<const char*>(buffer.getData()), buffer.getSize());
	}
};

BOOST_AUTO_TEST_CASE ( emptyBuffer ) {
	Buffer b;
	BOOST_CHECK (b.getData() != NULL);
	BOOST_CHECK_EQUAL (b.getSize(), 0u);
}

BOOST_AUTO_TEST_CASE ( bufferWithZeroInitialCapacity ) {
	Buffer b(0);
	BOOST_CHECK (b.getData() == NULL);
	BOOST_CHECK_EQUAL (b.getSize(), 0u);
}

BOOST_AUTO_TEST_CASE ( bufferWithNonZeroInitialCapacity ) {
	Buffer b(1);
	BOOST_CHECK (b.getData() != NULL);
	BOOST_CHECK_EQUAL (b.getSize(), 0u);
}

BOOST_AUTO_TEST_CASE ( increasingNullBuffer ) {
	BufferHelper t(0);
	BOOST_CHECK (t.buffer.getData() == NULL);
	BOOST_CHECK_EQUAL (t.buffer.getSize(), 0u);

	const std::string mystring("Hello World!");
	t.append(mystring);

	BOOST_CHECK (t.buffer.getData() != NULL);
	BOOST_CHECK_EQUAL (t.buffer.getSize(), mystring.size());
	BOOST_CHECK_EQUAL (mystring, t.asString());
}

BOOST_AUTO_TEST_CASE ( increasingNotNullBuffer ) {
	BufferHelper t;
	BOOST_CHECK (t.buffer.getData() != NULL);
	BOOST_CHECK_EQUAL (t.buffer.getSize(), 0u);

	t.append("Hello World!");

	BOOST_CHECK_EQUAL ("Hello World!", t.asString());
}

BOOST_AUTO_TEST_CASE ( increasingSeveralTimesWithNullBuffer ) {
	BufferHelper t(0);

	t.append("Dmitri");
	BOOST_CHECK_EQUAL ("Dmitri", t.asString());

	t.append(" Novikov");
	BOOST_CHECK_EQUAL ("Dmitri Novikov", t.asString());
}

BOOST_AUTO_TEST_CASE ( increasingSeveralTimesWithNotNullBuffer ) {
	BufferHelper t;

	t.append("First");
	BOOST_CHECK_EQUAL ("First", t.asString());

	t.append("Second");
	BOOST_CHECK_EQUAL ("FirstSecond", t.asString());
}

