#pragma once
#include "stdafx.h"
#include "ResimBuffer.h"
#include "ResimKey.h"

namespace Resim
{
	class Frame
	{
	public:
		Frame();
		Frame(size_t aFrameSize, size_t aNrOfKeys);

		void SaveVariable(void* aDataPtr, uint32_t aUID, size_t aSize);
		void* Get(uint32_t aUID, size_t aSize);
		void Clear();

		inline size_t HeapSize() const { return myBuffer.Size() + myKeys.size() * sizeof(FrameKey); }

	private:
		Buffer myBuffer;
		std::vector<FrameKey> myKeys;
	};
}