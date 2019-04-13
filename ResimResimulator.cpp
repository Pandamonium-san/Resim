#include "stdafx.h"
#include "ResimResimulator.h"

namespace Resim
{
	Resimulator* Resimulator::ourInstance = nullptr;

	void Resimulator::Create(size_t aMaxFrameSize, size_t aMaxNrFrames)
	{
		assert(ourInstance == nullptr && "Resimulator already created");
		ourInstance = new Resimulator(aMaxFrameSize, aMaxNrFrames);
	}

	void Resimulator::Destroy()
	{
		delete ourInstance;
		ourInstance = nullptr;
	}

	bool Resimulator::Exists()
	{
		return ourInstance != nullptr;
	}

	Resimulator* Resimulator::Get()
	{
		assert(ourInstance != nullptr && "Resimulator not created");
		return ourInstance;
	}

	void Resimulator::SaveFrame()
	{
		if (myCurrentFrameIndex != 0 && myCurrentFrameIndex < myFrames.Size() - 1)
		{
			return;
		}

		SignalPush();

		Frame diff = std::move(myDictionary.SaveFrameDiff(myPrevFrame));
		myPrevFrame = std::move(myDictionary.SaveFrame());
		myFrames.PushBack({ myFrameCounter++, std::move(diff) });
		myCurrentFrameIndex = static_cast<int>(myFrames.Size() - 1);
	}

	void Resimulator::SaveFullFrame()
	{
		if (myCurrentFrameIndex != 0 && myCurrentFrameIndex < myFrames.Size() - 1)
		{
			return;
		}

		SignalPush();

		myPrevFrame = std::move(myDictionary.SaveFrame());
		myFrames.PushBack({ myFrameCounter++, myPrevFrame });
		myCurrentFrameIndex = static_cast<int>(myFrames.Size() - 1);
	}

	void Resimulator::LoadFrame(size_t aNumber)
	{
		for (int i = 0; i < myFrames.Size(); i++)
		{
			if (myFrames[i].Number == aNumber)
			{
				LoadFrameIndex(i);
			}
		}
	}

	void Resimulator::LoadLatestFrame()
	{
		LoadFrame(myPrevFrame);
		myCurrentFrameIndex = static_cast<int>(myFrames.Size() - 1);
	}

	void Resimulator::LoadNextFrame()
	{
		LoadFrameIndex(myCurrentFrameIndex + 1);
	}

	void Resimulator::LoadPreviousFrame()
	{
		LoadFrameIndex(myCurrentFrameIndex - 1);
	}

	size_t Resimulator::GetTotalSizeInBytes() const
	{
		size_t totalSize = 0;

		totalSize += myVariables.size() * sizeof(MonitoredVariableInterface*);
		totalSize += myDictionary.HeapSize();
		totalSize += myPrevFrame.HeapSize();

		for (int i = 0; i < myFrames.Size(); i++)
		{
			totalSize += myFrames[i].Frame.HeapSize();
		}

		return totalSize;
	}

	void Resimulator::ClearBuffer()
	{
		myCurrentFrameIndex = 0;
		myFrameCounter = 0;
		myPrevFrame.Clear();
		myFrames.Clear();
	}

	void Resimulator::RemoveVariable(size_t aIndex)
	{
		myDictionary.RemoveVariable(aIndex);
		myVariables.erase(myVariables.begin() + aIndex);

		for (size_t i = aIndex; i < myVariables.size(); i++)
		{
			--myVariables[i]->myIndex;
			assert(i == myVariables[i]->myIndex);
		}
	}

	Resimulator::Resimulator(size_t aMaxFrameSize, size_t aMaxNrFrames) : myDictionary(aMaxFrameSize), myFrames(aMaxNrFrames)
	{
	}

	Resimulator::~Resimulator()
	{
	}

	void Resimulator::LoadFrameIndex(int aFrameIndex)
	{
		if (myCurrentFrameIndex == aFrameIndex)
		{
			return;
		}

		while (myCurrentFrameIndex < aFrameIndex)
		{
			if (myCurrentFrameIndex >= myFrames.Size() - 1)
			{
				break;
			}
			++myCurrentFrameIndex;
			LoadFrame(myFrames[myCurrentFrameIndex].Frame);
		}

		while (myCurrentFrameIndex > aFrameIndex)
		{
			if (myCurrentFrameIndex <= 0)
			{
				break;
			}
			--myCurrentFrameIndex;
			LoadFrame(myFrames[myCurrentFrameIndex].Frame);
		}
	}

	void Resimulator::LoadFrame(Frame& aFrame)
	{
		myDictionary.LoadFrame(aFrame);
		SignalPull();
	}

	void Resimulator::SignalPull()
	{
		for (auto var : myVariables)
		{
			var->ResimPull();
		}
	}

	void Resimulator::SignalPush()
	{
		for (auto var : myVariables)
		{
			var->ResimPush();
		}
	}
}