#pragma once
#include "stdafx.h"

namespace Resim
{
	class Interface
	{
	public:
		static void Create(uint32_t aMaxFrameSize, size_t aMaxNrFrames);
		static void Destroy();
		
		static void SaveFrame();
		static void SaveFullFrame();
		static void LoadFrame(size_t aNumber);
		static void LoadNextFrame();
		static void LoadPreviousFrame();
		static void LoadLatestFrame();

		static size_t GetNrFrames();
		static size_t GetMaxNrFrames();

		static size_t GetEarliestFrame();
		static size_t GetCurrentFrame();
		static size_t GetLatestFrame();

		static size_t GetNrVariablesTracked();
		static size_t GetTotalSizeInBytes();

		static void ClearBuffer();
	};
}
