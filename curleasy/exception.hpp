#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

#include <stdexcept>

namespace CurlEasy {

class Exception: public std::runtime_error {
public:
	explicit Exception(const std::string& message):std::runtime_error(message) {}
};

} // namespace CurlEasy

#endif /* EXCEPTION_HPP_ */
