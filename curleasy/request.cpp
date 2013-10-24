#include "request.hpp"
#include "exception.hpp"

namespace CurlEasy {

namespace {

inline void checkResult(CURLcode code) {
	if(code != CURLE_OK) {
		throw Exception(curl_easy_strerror(code));
	}
}

class GlobalContext {
	GlobalContext(const GlobalContext&) = delete;
	GlobalContext& operator=(const GlobalContext&) = delete;
public:
	GlobalContext() {
		checkResult (curl_global_init(CURL_GLOBAL_DEFAULT));
	}

	GlobalContext() {
		curl_global_cleanup();
	}

	void* newCurl() {
		CURL* c = curl_easy_init();
		if (!c) {
			throw Exception("Unable to initialize new curl request");
		}
		return c;
	}
} globalContext;

} // namespace


void Request::freeConextIfInitialized() {
	if (curlContext != NULL) {
		curl_easy_cleanup(curlContext);
	}
}

Request::Request(const std::string& url):curlContext(NULL), url(url) {}

void Request::init() {
	freeConextIfInitialized();
	curlContext = globalContext.newCurl();

	curl_easy_setopt(curlContext, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curlContext, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curlContext, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curlContext, CURLOPT_FOLLOWLOCATION, 1L);
}

void Request::perform() {
	checkResult (curl_easy_perform(curlContext));
}

void Request::initAndPerform() {
	init();
	perform();
}

Request::~Request() {
	freeConextIfInitialized();
}

} // namespace CurlEasy
