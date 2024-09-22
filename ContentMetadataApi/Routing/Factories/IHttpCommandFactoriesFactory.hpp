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

			virtual std::unique_ptr<IHttpCommandFactory> createHttpCommandFactory(const std::string& a_root_endpoint) const = 0;
		};
	}
}