#include "result.hpp"

namespace CurlEasy {

Result::Result(int code, const std::string& data):content(data), returnCode(code) {
}

Result::Result(Result&& other):returnCode(other.returnCode) {
	content.swap(other.content);
}

} // namespace CurlEasy
