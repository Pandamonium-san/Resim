#pragma once
#include "ResimProxy.h"
#include "ResimResimulator.h"
#include "ResimMonitoredVariableInterface.h"

namespace Resim
{
	// WARNING! Undefined behaviour if an object of this class is deleted through a T pointer, if T does not have a virtual destructor.
	template<class T>
	class MonitoredVariable : public Proxy<T>, MonitoredVariableInterface
	{
	public:
		friend class Resimulator;

		MonitoredVariable();
		~MonitoredVariable();
		MonitoredVariable(const MonitoredVariable& aMonitoredVariable);
		MonitoredVariable(MonitoredVariable&& aMonitoredVariable);
		MonitoredVariable& operator=(const MonitoredVariable& aMonitoredVariable);
		MonitoredVariable& operator=(MonitoredVariable&& aMonitoredVariable);

		MonitoredVariable(const T& aValue);
		MonitoredVariable(T&& aValue);
		MonitoredVariable& operator=(const T& aValue);
		MonitoredVariable& operator=(T&& aValue);

	protected:
		void ResimPush() override;
		void ResimPull() override;
	};


	template<class T>
	inline MonitoredVariable<T>::MonitoredVariable()
	{
		myIndex = Resim::Resimulator::Get()->AddVariable<T>(this, *this);
	}
	template<class T>
	inline MonitoredVariable<T>::~MonitoredVariable()
	{
		if (Resim::Resimulator::Exists() && myIndex != InvalidIndex)
		{
			Resim::Resimulator::Get()->RemoveVariable(myIndex);
		}
	}
	template<class T>
	inline MonitoredVariable<T>::MonitoredVariable(const MonitoredVariable& aMonitoredVariable)
	{
		Proxy<T>::operator=(aMonitoredVariable);
		myIndex = Resim::Resimulator::Get()->AddVariable<T>(this, *this);
	}
	template<class T>
	inline MonitoredVariable<T>::MonitoredVariable(MonitoredVariable&& aMonitoredVariable)
	{
		Proxy<T>::operator=(std::move(aMonitoredVariable));
		myIndex = aMonitoredVariable.myIndex;
		aMonitoredVariable.myIndex = InvalidIndex;
		Resim::Resimulator::Get()->UpdateReference(this);
	}
	template<class T>
	inline MonitoredVariable<T>& MonitoredVariable<T>::operator=(const MonitoredVariable& aMonitoredVariable)
	{
		Proxy<T>::operator=(aMonitoredVariable);
		return *this;
	}
	template<class T>
	inline MonitoredVariable<T>& MonitoredVariable<T>::operator=(MonitoredVariable&& aMonitoredVariable)
	{
		Proxy<T>::operator=(std::move(aMonitoredVariable));
		if (Resim::Resimulator::Exists() && myIndex != InvalidIndex)
		{
			Resim::Resimulator::Get()->RemoveVariable(myIndex);
		}
		myIndex = aMonitoredVariable.myIndex;
		aMonitoredVariable.myIndex = InvalidIndex;
		Resim::Resimulator::Get()->UpdateReference(this);
		return *this;
	}


	template<class T>
	inline MonitoredVariable<T>::MonitoredVariable(const T& aValue) : Proxy<T>(aValue)
	{
		myIndex = Resim::Resimulator::Get()->AddVariable<T>(this, *this);
	}
	template<class T>
	inline MonitoredVariable<T>::MonitoredVariable(T&& aValue) : Proxy<T>(std::forward<T>(aValue))
	{
		myIndex = Resim::Resimulator::Get()->AddVariable<T>(this, *this);
	}
	template<class T>
	inline MonitoredVariable<T>& MonitoredVariable<T>::operator=(const T& aValue)
	{
		Proxy<T>::operator=(aValue);
		return *this;
	}
	template<class T>
	inline MonitoredVariable<T>& MonitoredVariable<T>::operator=(T&& aValue)
	{
		Proxy<T>::operator=(std::forward<T>(aValue));
		return *this;
	}


	template<class T>
	inline void MonitoredVariable<T>::ResimPush()
	{
		Resim::Resimulator::Get()->SetData<T>(myIndex, *this);
	}
	template<class T>
	inline void MonitoredVariable<T>::ResimPull()
	{
		T& data = Resim::Resimulator::Get()->GetData<T>(myIndex);
		Proxy<T>::operator=(data);
	}
}
