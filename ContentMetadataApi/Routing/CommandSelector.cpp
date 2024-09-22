#include "CommandSelector.hpp"

namespace
{
    constexpr auto CONTENTS_ENDPOINT = "content";
    constexpr auto CONTENT_COMMENTS_ENDPOINT = "comment";
};

namespace ContentMetadataApi
{
    namespace Routing
    {
        CommandSelector::CommandSelector(std::shared_ptr<IHttpCommandFactoriesFactory> a_http_command_factories_factory) noexcept
            : m_http_command_factories_factory(a_http_command_factories_factory)
        {
        }

        std::unique_ptr<ICommand> CommandSelector::selectHttpCommand(const HttpRequest& a_request) const
        {
            auto http_command_factory = m_http_command_factories_factory->createHttpCommandFactory(a_request.m_root_endpoint);

            if (http_command_factory)
            {
                switch (a_request.m_method)
                {
                    case HttpMethod::Get: return http_command_factory->createGetCommand(a_request);
                    case HttpMethod::Post: return http_command_factory->createPostCommand(a_request);
                    case HttpMethod::Put: return http_command_factory->createPutCommand(a_request);
                    case HttpMethod::Delete: return http_command_factory->createDeleteCommand(a_request);
                    case HttpMethod::Patch: return http_command_factory->createPatchCommand(a_request);
                    default: return nullptr;
                }
            }

            return nullptr;
        }
    }
}