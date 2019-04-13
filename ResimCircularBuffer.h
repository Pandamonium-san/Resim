#pragma once
#include <vector>

namespace Resim
{
	template<class T>
	class CircularBuffer
	{
	public:
		CircularBuffer(size_t aSize)
		{
			myBuffer.reserve(aSize);
		}
		~CircularBuffer() = default;

		T& operator[](int aIndex) { return myBuffer[StepForward(myFront, aIndex)]; }
		T& Front() { return myBuffer[myFront]; }
		T& Back() { return myBuffer[myBack]; }
		const T& operator[](int aIndex) const { return myBuffer[StepForward(myFront, aIndex)]; }
		const T& Front() const { return myBuffer[myFront]; }
		const T& Back() const { return myBuffer[myBack]; }

		size_t Size() const { return myBuffer.size(); }
		size_t Capacity() const { return myBuffer.capacity(); }

		void PushBack(const T& aElement);
		void PushBack(T&& aElement);

		void Clear();

	private:
		int StepForward(int aItr, int aStep) const;
		int StepBack(int aItr, int aStep) const;

		std::vector<T> myBuffer;
		int myFront = 0;
		int myBack = -1;
	};
	template<class T>
	inline void CircularBuffer<T>::PushBack(const T& aElement)
	{
		myBack = StepForward(myBack, 1);
		if (Size() < Capacity())
		{
			myBuffer.push_back(aElement);
			return;
		}

		myFront = StepForward(myFront, 1);
		myBuffer[myBack] = aElement;
	}
	template<class T>
	inline void CircularBuffer<T>::PushBack(T&& aElement)
	{
		myBack = StepForward(myBack, 1);
		if (Size() < Capacity())
		{
			myBuffer.push_back(std::move(aElement));
			return;
		}

		myFront = StepForward(myFront, 1);
		myBuffer[myBack] = std::move(aElement);
	}
	template<class T>
	inline void CircularBuffer<T>::Clear()
	{
		myBuffer.clear();
		myFront = 0;
		myBack = -1;
	}
	template<class T>
	inline int CircularBuffer<T>::StepForward(int aItr, int aStep) const
	{
		return (aItr + aStep) % Capacity();
	}
	template<class T>
	inline int CircularBuffer<T>::StepBack(int aItr, int aStep) const
	{
		return ((aItr - aStep) + Capacity() * aStep) % Capacity();
	}
}