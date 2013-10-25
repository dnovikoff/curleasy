#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>

#include <boost/test/unit_test.hpp>

#include <curleasy/request.hpp>
#include <curleasy/exception.hpp>

		using namespace CurlEasy;

class TestHelper {
	const std::string getContent(const std::string& url) {
		Request r;
		return r.getContent(url);
	}
public:
	bool checkExc(const std::string& url, const std::string& expectedErrorMessage) {
		try {
			getContent(url);
		} catch(const Exception& e) {
			if (expectedErrorMessage == e.what()) return true;
			std::cout << "Expected error message: " << expectedErrorMessage << std::endl;
			std::cout << "Caught error message  : " << e.what() << std::endl;
			return false;
		}
		std::cout << "No Exception caught" << std::endl;
		return false;
	}

	bool checkContentContains(const std::string& url, const std::string& expectedContent) {
		using namespace CurlEasy;
		const std::string& receivedContent = getContent(url);
		if (receivedContent.find(expectedContent) != std::string::npos) return true;
		std::cout << "Expected content: " << expectedContent << std::endl;
		std::cout << "Received content: " << receivedContent << std::endl;
		return false;
	}
};

BOOST_AUTO_TEST_CASE ( unknownTest ) {
	TestHelper th;
	BOOST_CHECK( th.checkExc("unknownurl", "Couldn't resolve host name") );
}

BOOST_AUTO_TEST_CASE ( knonwHost ) {
	TestHelper th;
	BOOST_CHECK( th.checkContentContains("http://example.com", "<title>Example Domain</title>") );
}

BOOST_AUTO_TEST_CASE ( unknownPageOnKnownHost ) {
	TestHelper th;
	BOOST_CHECK( th.checkContentContains("http://example1.com/unknownpage", "TODO") );
}


