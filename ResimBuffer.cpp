#include "stdafx.h"
#include "ResimBuffer.h"

namespace Resim
{
	Buffer::Buffer(size_t aBytes)
	{
		myBuffer.resize(aBytes);
		mySize = 0;
		myMaxSize = aBytes;
	}

	size_t Buffer::Write(void* aData, size_t aBytes)
	{
		assert(mySize + aBytes <= myMaxSize && "Buffer is out of memory");

		size_t offset = mySize;
		char* dest = myBuffer.data() + offset;
		char* data = reinterpret_cast<char*>(aData);
		std::copy(data, data + aBytes, dest);
		mySize += aBytes;

		return offset;
	}

	void* Buffer::Read(size_t aOffset, size_t aBytes)
	{
		assert(aOffset + aBytes <= mySize && "Read is out of range");
		char* ptr = myBuffer.data() + aOffset;
		return ptr;
	}

	void Buffer::Set(size_t aOffset, void * aData, size_t aBytes)
	{
		assert(aOffset + aBytes <= myMaxSize && "Write is out of range");
		char* dest = myBuffer.data() + aOffset;
		char* data = reinterpret_cast<char*>(aData);
		std::copy(data, data + aBytes, dest);
	}

	size_t Buffer::Size() const
	{
		return mySize;
	}
	void Buffer::Remove(size_t aOffset, size_t aBytes)
	{
		assert(aOffset + aBytes <= mySize && "Read is out of range");

		char* ptr = myBuffer.data() + aOffset;
		char* end = myBuffer.data() + mySize;

		std::copy(ptr + aBytes, end, ptr);
		mySize -= aBytes;
	}
	void Buffer::Clear()
	{
		mySize = 0;
		myBuffer.clear();
	}
}