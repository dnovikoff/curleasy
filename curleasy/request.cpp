#include <iostream>
#include <assert.h>

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


void Request::freeConextIfInitialized() {
	if (curlContext != NULL) {
		curl_easy_cleanup(curlContext);
	}
}

Request::Request(const std::string& url):curlContext(NULL), url(url) {
	buffer.reserve(DEFAULT_INITIAL_BUFFER_SIZE);
}

void Request::init() {
	freeConextIfInitialized();
	curlContext = globalContext.newCurl();

	curl_easy_setopt(curlContext, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curlContext, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curlContext, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curlContext, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curlContext, CURLOPT_WRITEFUNCTION, curlWriteCallback);
	curl_easy_setopt(curlContext, CURLOPT_WRITEDATA, this);
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

size_t Request::curlWriteCallback(char* data, size_t symbolSize, size_t symbolCount, void* t) {
	assert( symbolCount != 0 );
	assert( symbolSize != 0 );
	assert( data != NULL );

	std::cout << "Test " << symbolSize << "*" << symbolCount << std::endl;
	Request* ths = reinterpret_cast<Request*>(t);
	const size_t size = symbolSize*symbolCount;
	ths->appendData(data, size);
	return size;
}

void Request::appendData(char* data, size_t size) {
	buffer.append(data, size);
}

const std::string& Request::toString() const {
	return buffer;
}

} // namespace CurlEasy
