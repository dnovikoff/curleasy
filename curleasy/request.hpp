#ifndef REQUEST_HPP_
#define REQUEST_HPP_

#include <string>

namespace CurlEasy {
class Result;

class Request {
	friend class RequestConfigurator;
	enum { DEFAULT_INITIAL_BUFFER_SIZE = 1024 };
public:
	Request();

	Result requestUrl(const std::string& url);

	virtual ~Request();
protected:
	void setUserAgent(const std::string& userAgent);
	void disableVerifySSL();
	void acceptAllEncodings();
	void enableAutoReferer();
	void enableAutoCookies();
	void setTimeoutSeconds(size_t seconds);
private:
	void init();
	virtual void configure() = 0;
	static size_t curlWriteCallback(char* data, size_t symbolSize, size_t symbolCount, void* ths);
	void appendData(char* data, size_t size);

	// noncopy
	Request(const Request&) = delete;
	Request& operator=(const Request&) = delete;

	void * const curlContext;
	bool initialized;
	std::string buffer;
};

} // namespace CurlEasy

#endif /* REQUEST_HPP_ */
