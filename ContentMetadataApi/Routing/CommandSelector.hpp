#pragma once
#include "ICommandSelector.hpp"
#include "Factories/IHttpCommandFactoriesFactory.hpp"

namespace ContentMetadataApi
{
	namespace Routing 
	{
		class CONTENT_METADATA_API_EXPORT CommandSelector : public ICommandSelector
		{
		public:
			explicit CommandSelector(std::shared_ptr<IHttpCommandFactoriesFactory> a_http_command_factories_factory) noexcept;
			
			CommandSelector(const CommandSelector&) = delete;
			CommandSelector(CommandSelector&&) = delete;

			CommandSelector& operator=(const CommandSelector&) = delete;
			CommandSelector& operator=(CommandSelector&&) = delete;

			std::unique_ptr<ICommand> selectHttpCommand(const HttpRequest& a_request) const override;

		private:
			std::shared_ptr<IHttpCommandFactoriesFactory> m_http_command_factories_factory;

		};
	}
}