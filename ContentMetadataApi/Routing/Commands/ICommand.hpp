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

			virtual boost::asio::awaitable<HttpResponse> executeAsync() const = 0;
		};
	}
}