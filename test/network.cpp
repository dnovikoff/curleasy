#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>

#include <boost/test/unit_test.hpp>

#include <curleasy/request.hpp>
#include <curleasy/exception.hpp>

class TestHelper {
public:
	bool checkExc(const std::string& url, const std::string& expectedErrorMessage) {
		using namespace CurlEasy;
		Request r(url);
		try {
			r.initAndPerform();
		} catch(const Exception& e) {
			if (expectedErrorMessage == e.what()) return true;
			std::cout << "Expected error message: " << expectedErrorMessage << std::endl;
			std::cout << "Caught error message  : " << e.what() << std::endl;
			return false;
		}
		std::cout << "No Exception caught" << std::endl;
		return false;
	}
};

BOOST_AUTO_TEST_CASE ( unknownTest ) {
	TestHelper th;
	BOOST_CHECK( th.checkExc("unknownurl", "Couldn't resolve host name") );
}


