#ifndef REQUEST_HPP_
#define REQUEST_HPP_

#include <curl/curl.h>
#include <string>

namespace CurlEasy {

class Request {
	enum { DEFAULT_INITIAL_BUFFER_SIZE = 1024 };
public:
	explicit Request(const std::string& url);

	void initAndPerform();
	void init();
	void perform();

	~Request();

	static size_t curlWriteCallback(char* data, size_t symbolSize, size_t symbolCount, void* ths);
	void appendData(char* data, size_t size);
	const std::string& toString() const;
private:
	void *curlContext;
	const std::string url;
	std::string buffer;

	void freeConextIfInitialized();

	Request(const Request&) = delete;
	Request& operator=(const Request&) = delete;
};

} // namespace CurlEasy

#endif /* REQUEST_HPP_ */
