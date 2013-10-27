#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>

#include <boost/test/unit_test.hpp>

#include <curleasy/request.hpp>
#include <curleasy/exception.hpp>
#include <curleasy/result.hpp>

using namespace CurlEasy;

class TestRequest: public Request {
	void configure() override {
		disableVerifySSL();
		setUserAgent("Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:24.0) Gecko/20100101 Firefox/24.0");
		acceptAllEncodings();
		enableAutoCookies();
	}
};

class TestHelper {
	Result getResult(const std::string& url) {
		TestRequest r;
		return r.requestUrl(url);
	}
	const std::string getContent(const std::string& url) {
		return getResult(url).getData();
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

	bool checkContentContains(const std::string& url, const std::string& expectedContent, int expectedCode = Result::OK_CODE) {
		using namespace CurlEasy;
		Result result(getResult(url));
		if (expectedCode != result.getCode()) {
			std::cout << "Expected code: " << expectedCode << std::endl;
			std::cout << "Received code: " << result.getCode() << std::endl;
			return false;
		}
		const std::string& receivedContent = getContent(url);
		if (receivedContent.find(expectedContent) != std::string::npos) return true;
		std::cout << "Expected content: " << expectedContent << std::endl;
		std::cout << "Received content: " << receivedContent << std::endl;
		return false;
	}

	bool checkCode(const std::string& url, int expectedCode) {
		using namespace CurlEasy;
		Result result(getResult(url));
		if (expectedCode != result.getCode()) {
			std::cout << "Expected code: " << expectedCode << std::endl;
			std::cout << "Received code: " << result.getCode() << std::endl;
			return false;
		}
		return true;
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
	BOOST_CHECK( th.checkCode("http://example1.com/unknownpage", Result::NOT_FOUND_CODE) );
}
