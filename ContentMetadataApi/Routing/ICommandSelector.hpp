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

			virtual std::unique_ptr<ICommand> selectHttpCommand(const HttpRequest& a_request) const = 0;
		};
	}
}