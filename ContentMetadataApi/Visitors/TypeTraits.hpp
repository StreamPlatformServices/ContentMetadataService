#pragma once
#include "../StdAfx.hpp"
#include <type_traits>

namespace ContentMetadataApi
{
	namespace Visitors
	{
		////CPP 11-
		//template<typename T>
		//struct is_optional : std::false_type {};

		//template<typename T>
		//struct is_optional<boost::optional<T>> : std::true_type {};

		////CPP 14+
		//template<typename T>
		//constexpr bool is_optional = false;

		//template<typename T>
		//constexpr bool is_optional<boost::optional<T>> = true;

		////CPP 17+
		//template<typename T>
		//constexpr bool is_optional = false;

		//template<typename T>
		//constexpr bool is_optional<std::optional<T>> = true;

		//CPP 20+
		template<typename T>
		concept is_optional = std::is_same_v<T, std::optional<typename T::value_type>>;

		//Usage: is_optional<decltype(object)>
	}
}