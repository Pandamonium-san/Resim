#pragma once

namespace Resim
{
	class MonitoredVariableInterface
	{
		friend class Resimulator;
	protected:
		virtual void ResimPush() = 0;
		virtual void ResimPull() = 0;

		MonitoredVariableInterface() = default;
		virtual ~MonitoredVariableInterface() = default;

		static constexpr size_t InvalidIndex = static_cast<size_t>(-1);
		size_t myIndex = InvalidIndex;
	};
}
