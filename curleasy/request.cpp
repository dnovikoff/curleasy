#include <iostream>
#include <assert.h>

#include <curl/curl.h>

#include "request.hpp"
#include "exception.hpp"
#include "result.hpp"

namespace CurlEasy {

namespace {

inline void checkResult(CURLcode code) {
	if(code != CURLE_OK) {
		throw Exception(curl_easy_strerror(code));
	}
}

#define setOpt(KEY, VALUE) checkResult (curl_easy_setopt(curlContext, KEY, VALUE))

class GlobalContext {
	GlobalContext(const GlobalContext&) = delete;
	GlobalContext& operator=(const GlobalContext&) = delete;
public:
	GlobalContext() {
		checkResult (curl_global_init(CURL_GLOBAL_DEFAULT));
	}

	~GlobalContext() {
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


Request::Request():curlContext(globalContext.newCurl()),initialized(false) {
}

void Request::setUserAgent(const std::string& userAgent) {
	setOpt(CURLOPT_USERAGENT, userAgent.c_str());
}

void Request::disableVerifySSL() {
	setOpt(CURLOPT_SSL_VERIFYPEER, 0L);
	setOpt(CURLOPT_SSL_VERIFYHOST, 0L);
}

void Request::acceptAllEncodings() {
	setOpt(CURLOPT_ENCODING, "");
}

void Request::enableAutoReferer() {
	setOpt(CURLOPT_AUTOREFERER, 1L);
}

void Request::enableAutoCookies() {
	setOpt(CURLOPT_COOKIELIST, "");
}

void Request::init() {
	buffer.reserve(DEFAULT_INITIAL_BUFFER_SIZE);

	configure();

	setOpt(CURLOPT_FOLLOWLOCATION, 1L);

	setOpt(CURLOPT_WRITEFUNCTION, curlWriteCallback);
	setOpt(CURLOPT_WRITEDATA, this);
}

Result Request::requestUrl(const std::string& url) {
	if (!initialized) {
		init();
		initialized = true;
	}
	buffer.clear();
	curl_easy_setopt(curlContext, CURLOPT_URL, url.c_str());

	checkResult (curl_easy_perform(curlContext));
	long respCode = 0;
	checkResult (curl_easy_getinfo(curlContext, CURLINFO_RESPONSE_CODE, &respCode));

	return Result( static_cast<int>(respCode), buffer);
}

Request::~Request() {
	curl_easy_cleanup(curlContext);
}

size_t Request::curlWriteCallback(char* data, size_t symbolSize, size_t symbolCount, void* t) {
	assert( symbolCount != 0 );
	assert( symbolSize != 0 );
	assert( data != NULL );

	Request* ths = reinterpret_cast<Request*>(t);
	const size_t size = symbolSize*symbolCount;
	ths->appendData(data, size);
	return size;
}

void Request::appendData(char* data, size_t size) {
	buffer.append(data, size);
}

} // namespace CurlEasy
