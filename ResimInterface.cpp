#include "stdafx.h"
#include "ResimInterface.h"
#include "ResimResimulator.h"

namespace Resim
{
	void Interface::Create(uint32_t aMaxFrameSize, size_t aMaxNrFrames)
	{
		Resimulator::Create(aMaxFrameSize, aMaxNrFrames);
	}

	void Interface::Destroy()
	{
		Resimulator::Destroy();
	}

	void Interface::SaveFrame()
	{
		Resimulator::Get()->SaveFrame();
	}

	void Interface::SaveFullFrame()
	{
		Resimulator::Get()->SaveFullFrame();
	}

	void Interface::LoadFrame(size_t aNumber)
	{
		Resimulator::Get()->LoadFrame(aNumber);
	}

	void Interface::LoadNextFrame()
	{
		Resimulator::Get()->LoadNextFrame();
	}

	void Interface::LoadPreviousFrame()
	{
		Resimulator::Get()->LoadPreviousFrame();
	}

	void Interface::LoadLatestFrame()
	{
		Resimulator::Get()->LoadLatestFrame();
	}
	size_t Interface::GetNrFrames()
	{
		return Resimulator::Get()->GetNrFrames();
	}
	size_t Interface::GetMaxNrFrames()
	{
		return Resimulator::Get()->GetMaxNrFrames();
	}
	size_t Interface::GetEarliestFrame()
	{
		return Resimulator::Get()->GetEarliestFrame();
	}
	size_t Interface::GetCurrentFrame()
	{
		return Resimulator::Get()->GetCurrentFrame();
	}
	size_t Interface::GetLatestFrame()
	{
		return Resimulator::Get()->GetLatestFrame();
	}
	size_t Interface::GetNrVariablesTracked()
	{
		return Resimulator::Get()->GetNrVariablesTracked();
	}
	size_t Interface::GetTotalSizeInBytes()
	{
		return Resimulator::Get()->GetTotalSizeInBytes();
	}
	void Interface::ClearBuffer()
	{
		Resimulator::Get()->ClearBuffer();
	}
}