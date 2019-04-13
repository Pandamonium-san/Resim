#pragma once
#include <vector>

namespace Resim
{
	class Buffer
	{
	public:
		Buffer(size_t aBytes);

		size_t Write(void* aData, size_t aBytes);
		template<class T>
		size_t Write(const T& aData);
		template<class T>
		size_t Write(T&& aData);

		void* Read(size_t aOffset, size_t aBytes);
		template<class T>
		T& Read(size_t aOffset);

		void Set(size_t aOffset, void* aData, size_t aBytes);
		void Remove(size_t aOffset, size_t aBytes);
		void Clear();
		size_t Size() const;

	private:
		size_t mySize;
		size_t myMaxSize;
		std::vector<char> myBuffer;
	};

	template<class T>
	inline size_t Buffer::Write(const T& aData)
	{
		assert(myMaxSize >= mySize + sizeof(T) && "Buffer is out of memory");

		size_t offset = mySize;
		char* ptr = myBuffer.data() + offset;
		new(ptr) T(aData);
		mySize += sizeof(T);

		return offset;
	}
	template<class T>
	inline size_t Buffer::Write(T&& aData)
	{
		assert(myMaxSize >= mySize + sizeof(T) && "Buffer is out of memory");

		size_t offset = mySize;
		char* ptr = myBuffer.data() + offset;
		new(ptr) T(std::forward<T>(aData));
		mySize += sizeof(T);

		return offset;
	}

	template<class T>
	inline T& Buffer::Read(size_t aOffset)
	{
		void* data = Read(aOffset, sizeof(T));
		T* tptr = reinterpret_cast<T*>(data);
		return *tptr;
	}
}