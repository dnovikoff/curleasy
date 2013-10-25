#ifndef REQUEST_HPP_
#define REQUEST_HPP_

#include <curl/curl.h>
#include <string>

namespace CurlEasy {

class Request {
	enum { DEFAULT_INITIAL_BUFFER_SIZE = 1024 };
public:
	Request();

	const std::string& getContent(const std::string& url);

	~Request();
private:
	void init();
	static size_t curlWriteCallback(char* data, size_t symbolSize, size_t symbolCount, void* ths);
	void appendData(char* data, size_t size);

	// noncopy
	Request(const Request&) = delete;
	Request& operator=(const Request&) = delete;

	void * const curlContext;
	std::string buffer;
};

} // namespace CurlEasy

#endif /* REQUEST_HPP_ */
