#pragma once
#include "../../StdAfx.hpp"
#include "IHttpCommandFactory.hpp"

namespace ContentMetadataApi
{
	namespace Routing
	{
		class IHttpCommandFactoriesFactory
		{
		public:
			virtual ~IHttpCommandFactoriesFactory() = default;

			virtual auto createHttpCommandFactory(const std::string& a_root_endpoint) const -> std::unique_ptr<IHttpCommandFactory> = 0;
		};
	}
}