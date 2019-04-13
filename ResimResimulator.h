#pragma once
#include "ResimCircularBuffer.h"
#include "ResimDictionary.h"
#include "ResimMonitoredVariableInterface.h"

namespace Resim
{
	class Resimulator
	{
	public:
		static void Create(size_t aBufferSize, size_t aMaxNrFrames);
		static void Destroy();
		static bool Exists();
		static Resimulator* Get();

		void SaveFrame();
		void SaveFullFrame();

		void LoadFrame(size_t aNumber);
		void LoadLatestFrame();
		void LoadNextFrame();
		void LoadPreviousFrame();

		inline size_t GetNrFrames() const { return myFrames.Size(); }
		inline size_t GetMaxNrFrames() const { return myFrames.Capacity(); }

		inline size_t GetEarliestFrame() const { return myFrames.Front().Number; }
		inline size_t GetCurrentFrame() const { return myFrames[myCurrentFrameIndex].Number; }
		inline size_t GetLatestFrame() const { return myFrames.Back().Number; }

		size_t GetTotalSizeInBytes() const;
		size_t GetNrVariablesTracked() const { return myVariables.size(); }

		void ClearBuffer();

		template<class T>
		size_t AddVariable(MonitoredVariableInterface* aVar, const T& aData);
		template<class T>
		size_t AddVariable(MonitoredVariableInterface* aVar, T&& aData);
		void RemoveVariable(size_t aIndex);

		inline void UpdateReference(MonitoredVariableInterface* aVar) { myVariables[aVar->myIndex] = aVar; }

		template<class T>
		void SetData(size_t aIndex, const T& aData);
		template<class T>
		T& GetData(size_t aIndex);

	private:
		static Resimulator* ourInstance;

		Resimulator(size_t aMaxFrameSize, size_t aMaxNrFrames);
		~Resimulator();

		std::vector<MonitoredVariableInterface*> myVariables;

		int myCurrentFrameIndex = 0;
		Dictionary myDictionary;
		Frame myPrevFrame;

		size_t myFrameCounter = 0;
		struct CountedFrame
		{
			size_t Number;
			Frame Frame;
		};
		CircularBuffer<CountedFrame> myFrames;

		void LoadFrameIndex(int aFrameIndex);
		void LoadFrame(Frame& aFrame);
		void SignalPull();
		void SignalPush();
	};

	template<class T>
	inline size_t Resimulator::AddVariable(MonitoredVariableInterface* aVar, const T& aData)
	{
		size_t index = myDictionary.AddVariable<T>(aData);
		myVariables.push_back(aVar);
		return index;
	}

	template<class T>
	inline size_t Resimulator::AddVariable(MonitoredVariableInterface* aVar, T&& aData)
	{
		size_t index = myDictionary.AddVariable<T>(std::forward<T>(aData));
		myVariables.push_back(aVar);
		return index;
	}

	template<class T>
	inline void Resimulator::SetData(size_t aIndex, const T& aData)
	{
		myDictionary.SetData<T>(aIndex, aData);
	}

	template<class T>
	inline T& Resimulator::GetData(size_t aIndex)
	{
		return myDictionary.GetData<T>(aIndex);
	}
}