#include "stdafx.h"
#include "ResimFrame.h"

namespace Resim
{
	Frame::Frame() : myBuffer(0), myKeys(0)
	{
	}

	Frame::Frame(size_t aFrameSize, size_t aNrOfKeys) : myBuffer(aFrameSize)
	{
		myKeys.reserve(aNrOfKeys);
	}

	void* Frame::Get(uint32_t aUID, size_t aSize)
	{
		const FrameKey key = { aUID, 0 };
		auto itr = std::lower_bound(myKeys.begin(), myKeys.end(), key);
		if (itr != myKeys.end() && aUID == itr->UID)
		{
			return myBuffer.Read(itr->StreamOffset, aSize);
		}
		return nullptr;
	}

	void Frame::SaveVariable(void* aDataPtr, uint32_t aUID, size_t aSize)
	{
		const size_t offset = myBuffer.Write(aDataPtr, aSize);
		myKeys.push_back({ aUID, offset });
	}
	void Frame::Clear()
	{
		myBuffer.Clear();
		myKeys.clear();
	}
}