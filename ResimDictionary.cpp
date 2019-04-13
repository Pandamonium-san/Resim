#include "stdafx.h"
#include "ResimDictionary.h"

namespace Resim
{
	Dictionary::Dictionary(size_t aDictionarySize) : myBuffer(aDictionarySize)
	{
	}

	void Dictionary::RemoveVariable(size_t aIndex)
	{
		DictionaryKey removed = myKeys[aIndex];
		myKeys.erase(myKeys.begin() + aIndex);
		myBuffer.Remove(removed.StreamOffset, removed.Size);

		for (size_t i = aIndex; i < myKeys.size(); i++)
		{
			myKeys[i].StreamOffset -= removed.Size;
		}
	}

	void Dictionary::LoadFrame(Frame& aFrame)
	{
		for (DictionaryKey& key : myKeys)
		{
			void* data = aFrame.Get(key.UID, key.Size);
			if (data != nullptr)
			{
				myBuffer.Set(key.StreamOffset, data, key.Size);
			}
		}
	}

	Frame Dictionary::SaveFrame()
	{
		Frame frame(myBuffer.Size(), myKeys.size());
		for (DictionaryKey& key : myKeys)
		{
			void* dataPtr = myBuffer.Read(key.StreamOffset, key.Size);
			frame.SaveVariable(dataPtr, key.UID, key.Size);
		}
		return frame;
	}

	Frame Dictionary::SaveFrameDiff(Frame& aPrevious)
	{
		struct KeyData
		{
			void* Data;
			uint32_t UID;
			size_t Size;
		};

		size_t frameSize = 0;
		thread_local static std::vector<KeyData> changedKeys;
		changedKeys.clear();
		changedKeys.reserve(myKeys.size());

		for (DictionaryKey& key : myKeys)
		{
			void* dataPtr = myBuffer.Read(key.StreamOffset, key.Size);
			void* prev = aPrevious.Get(key.UID, key.Size);

			if (prev == nullptr || std::memcmp(dataPtr, prev, key.Size) != 0)
			{
				frameSize += key.Size;
				changedKeys.push_back({ dataPtr, key.UID, key.Size });
			}
		}

		Frame frame(frameSize, changedKeys.size());
		for (KeyData& keyData : changedKeys)
		{
			frame.SaveVariable(keyData.Data, keyData.UID, keyData.Size);
		}

		return frame;
	}
	void Dictionary::Clear()
	{
		myUIDCounter = 0;
		myBuffer.Clear();
		myKeys.clear();
	}
}