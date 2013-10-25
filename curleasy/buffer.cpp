#include <memory.h>

#include "buffer.hpp"

namespace CurlEasy {

	void* data;
	size_t size;
	size_t capacity;

Buffer::Buffer(size_t initialSize):data(NULL), size(0), capacity(initialSize) {
	if (capacity == 0) return;
	data = malloc(capacity);
}

bool Buffer::tryAppendData(size_t nextSize, void* nextData) {
	const size_t newSize = size + nextSize;
	if (newSize>=capacity) {
		const size_t newCapacity = newSize + 1;
		void* const newData = realloc(data, capacity);
		if (!newData) return false;
		data = newData;
		capacity = newCapacity;
	}
	memcpy(reinterpret_cast<char*>(data)+size, nextData, nextSize);
	size = newSize;
	return true;
}

Buffer::~Buffer() {
	free(data);
}


} // namespace CurlEasy
