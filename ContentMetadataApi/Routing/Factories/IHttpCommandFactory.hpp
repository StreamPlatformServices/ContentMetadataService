#pragma once
#include "../../StdAfx.hpp"
#include "../../HttpDataStructures/HttpRequest.hpp"
#include "../Commands/ICommand.hpp"

namespace ContentMetadataApi
{
	namespace Routing
	{
		class IHttpCommandFactory
		{
		public:
			virtual ~IHttpCommandFactory() = default;

			virtual std::unique_ptr<ICommand> createGetCommand(const HttpRequest& a_request) const = 0;
			virtual std::unique_ptr<ICommand> createPostCommand(const HttpRequest& a_request) const = 0;
			virtual std::unique_ptr<ICommand> createPutCommand(const HttpRequest& a_request) const = 0;
			virtual std::unique_ptr<ICommand> createDeleteCommand(const HttpRequest& a_request) const = 0;
			virtual std::unique_ptr<ICommand> createPatchCommand(const HttpRequest& a_request) const = 0;
		};
	}
}