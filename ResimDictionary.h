#pragma once
#include "stdafx.h"
#include "ResimBuffer.h"
#include "ResimFrame.h"

#include <typeinfo>

namespace Resim
{
	class Dictionary
	{
	public:
		Dictionary(size_t aDictionarySize);

		template<class T>
		size_t AddVariable(const T& aData);
		template<class T>
		size_t AddVariable(T&& aData);

		void RemoveVariable(size_t aIndex);

		template<class T>
		void SetData(size_t aIndex, const T& aData);
		template<class T>
		T& GetData(size_t aIndex);

		Frame SaveFrame();
		Frame SaveFrameDiff(Frame& aPrevious);
		void LoadFrame(Frame& aFrame);

		void Clear();
		inline size_t HeapSize() const { return myBuffer.Size() + myKeys.capacity() * sizeof(DictionaryKey); }

	private:
		uint32_t myUIDCounter = 0;
		Buffer myBuffer;
		std::vector<DictionaryKey> myKeys;
	};

	template<class T>
	inline size_t Dictionary::AddVariable(const T& aData)
	{
		size_t offset = myBuffer.Write<T>(aData);

		DictionaryKey key;
		key.UID = myUIDCounter++;
		key.StreamOffset = offset;
		key.Size = sizeof(T);
		key.TypeName = typeid(T).name();

		myKeys.push_back(key);

		return myKeys.size() - 1;
	}
	template<class T>
	inline size_t Dictionary::AddVariable(T&& aData)
	{
		size_t offset = myBuffer.Write<T>(std::forward<T>(aData));

		DictionaryKey key;
		key.UID = myUIDCounter++;
		key.StreamOffset = offset;
		key.Size = sizeof(T);
		key.TypeName = typeid(T).name();

		myKeys.push_back(key);

		return myKeys.size() - 1;
	}
	template<class T>
	inline void Dictionary::SetData(size_t aIndex, const T& aData)
	{
		GetData<T>(aIndex) = aData;
	}
	template<class T>
	inline T& Dictionary::GetData(size_t aIndex)
	{
		DictionaryKey& key = myKeys[aIndex];
#ifdef _DEBUG
		const char* typeName = typeid(T).name();
		assert(key.TypeName == typeName && "Invalid type");
#endif
		return myBuffer.Read<T>(key.StreamOffset);
	}
}