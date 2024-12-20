#pragma once
#include "../../StdAfx.hpp"
#include "../../HttpDataStructures/HttpResponse.hpp"

namespace ContentMetadataApi
{
	namespace Routing
	{
		class ICommand
		{
		public:
			virtual ~ICommand() = default;

			virtual auto executeAsync() const -> boost::asio::awaitable<HttpResponse> = 0;
		};
	}
}