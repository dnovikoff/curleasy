#ifndef REQUEST_HPP_
#define REQUEST_HPP_

#include <curl/curl.h>
#include <string>

namespace CurlEasy {

class Request {
public:
	explicit Request(const std::string& url);

	void initAndPerform();
	void init();
	void perform();

	~Request();
private:
	void *curlContext;
	const std::string url;

	void freeConextIfInitialized();

	Request(const Request&) = delete;
	Request& operator=(const Request&) = delete;
};

} // namespace CurlEasy

#endif /* REQUEST_HPP_ */
