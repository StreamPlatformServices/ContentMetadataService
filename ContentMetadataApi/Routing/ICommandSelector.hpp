#pragma once
#include "../StdAfx.hpp"
#include "Commands/ICommand.hpp"
#include "../HttpDataStructures/HttpRequest.hpp"

namespace ContentMetadataApi
{
	namespace Routing
	{
		class ICommandSelector
		{
			
		public:
			virtual ~ICommandSelector() = default;

			virtual auto selectHttpCommand(const HttpRequest& a_request) const -> std::unique_ptr<ICommand> = 0;
		};
	}
}