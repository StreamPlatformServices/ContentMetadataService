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

			virtual auto createGetCommand(const HttpRequest& a_request) const -> std::unique_ptr<ICommand> = 0;
			virtual auto createPostCommand(const HttpRequest& a_request) const -> std::unique_ptr<ICommand> = 0;
			virtual auto createPutCommand(const HttpRequest& a_request) const -> std::unique_ptr<ICommand> = 0;
			virtual auto createDeleteCommand(const HttpRequest& a_request) const -> std::unique_ptr<ICommand> = 0;
			virtual auto createPatchCommand(const HttpRequest& a_request) const -> std::unique_ptr<ICommand> = 0;
		};
	}
}