#pragma once
#include <type_traits>

namespace Resim
{
	template<class T, class Enable = void>
	class Proxy {};

	template<class T>
	class Proxy<T, typename std::enable_if<!std::is_fundamental_v<T>>::type> : public T
	{
	public:
		Proxy() : T() {}
		virtual ~Proxy() {}
		Proxy(const T& aValue) : T(aValue) {}
		Proxy(T&& aValue) : T(std::forward<T>(aValue)) {}
		Proxy& operator=(const T& aValue) { T::operator=(aValue); return *this; }
		Proxy& operator=(T&& aValue) { T::operator=(std::forward<T>(aValue)); return *this; }
	};

	template<class T>
	class Proxy<T, typename std::enable_if<std::is_fundamental_v<T>>::type>
	{
		T value;
	public:
		Proxy() : value() {}
		virtual ~Proxy() {}
		Proxy(T aValue) : value(aValue) {}
		Proxy& operator=(T aValue) { value = aValue; return *this; }

		operator T&() { return value; }
		operator const T&() const { return value; }
	};
}