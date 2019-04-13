#pragma once
#include <stdint.h>

namespace Resim
{
	class MonitoredVariableInterface;

	struct FrameKey
	{
		uint32_t UID;
		size_t StreamOffset;

		inline bool operator<(const FrameKey& aOther) { return UID < aOther.UID; }
		inline bool operator==(const FrameKey& aOther) { return UID == aOther.UID; }
	};
	struct DictionaryKey 
	{
		uint32_t UID;
		size_t StreamOffset;
		size_t Size;
		const char* TypeName;

		inline bool operator<(const DictionaryKey& aOther) { return UID < aOther.UID; }
		inline bool operator==(const DictionaryKey& aOther) { return UID == aOther.UID; }
	};
}