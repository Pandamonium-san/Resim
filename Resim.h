#pragma once
#include "ResimInterface.h"
#include "ResimMonitoredVariable.h"

namespace Resim
{
#ifdef RESIM_DISABLE
	template<class T>
	using Var = Resim::Proxy<T>;
#else
	template<class T>
	using Var = Resim::MonitoredVariable<T>;
#endif
}

#define RESIM_VAR(type, name) Resim::Var<type> name;
