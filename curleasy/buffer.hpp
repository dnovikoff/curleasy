#ifndef BUFFER_HPP_
#define BUFFER_HPP_

#include <stdexcept>

namespace CurlEasy {

class Buffer {
	Buffer(const Buffer&) = delete;
	Buffer& operator=(const Buffer&) = delete;

	void* data;
	size_t size;
	size_t capacity;
public:
	enum { DEFAULT_INITIAL_SIZE = 1024 };

	explicit Buffer(size_t initialSize = DEFAULT_INITIAL_SIZE);
	bool tryAppendData(size_t nextSize, void* nextData);

	void* getData() const { return data; }
	size_t getSize() const { return size; }

	~Buffer();
};


} // namespace CurlEasy


#endif /* BUFFER_HPP_ */
