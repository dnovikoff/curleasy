#ifndef CURLEASY_RESULT_HPP_
#define CURLEASY_RESULT_HPP_

#include <string>

namespace CurlEasy {

class Result {
	Result(const Result&) = delete;
	Result& operator=(const Result&) = delete;

	std::string content;
	int returnCode;
public:
	enum {
		OK_CODE = 200,
		NOT_FOUND_CODE = 404,
	};

	Result(int code, const std::string& data);
	Result(Result&& other);
	Result& operator=(Result&& other);
	const std::string& getData() const {
		return content;
	}
	int getCode() const {
		return returnCode;
	}
	bool isOKCode() const {
		return getCode() == OK_CODE;
	}
};

} // namespace CurlEasy

#endif // CURLEASY_RESULT_HPP_
