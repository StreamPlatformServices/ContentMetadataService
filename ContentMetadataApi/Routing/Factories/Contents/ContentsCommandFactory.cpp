#include "ContentsCommandFactory.hpp"
#include "../../Commands/Contents/AddContentCommand.hpp"
#include "../../Commands/Contents/UpdateContentCommand.hpp"
#include "../../Commands/Contents/DeleteContentCommand.hpp"
#include "../../Commands/Contents/GetContentByIdCommand.hpp"
#include "../../Commands/Contents/GetContentsCommand.hpp"
#include "../../Commands/Contents/GetContentsByOwnerIdCommand.hpp"

namespace ContentMetadataApi
{
    namespace Routing
    {
        namespace
        {
            constexpr auto AUTHOR_ROUTING_PATH_VALUE = "author";
        }

        ContentsCommandFactory::ContentsCommandFactory(
            std::shared_ptr<ContentMetadataCore::IContentsContract> a_contents_contract,
            std::shared_ptr<Mappers::IContentDataMapper> a_content_mapper,
            std::shared_ptr<Parsers::IGuidParser> a_guid_parser,
            std::shared_ptr<Visitors::IVisitorFactory>	a_visitor_factory) noexcept
            : m_contents_contract(a_contents_contract)
            , m_content_mapper(a_content_mapper)
            , m_guid_parser(a_guid_parser)
            , m_visitor_factory(a_visitor_factory)
        {
        }

        std::unique_ptr<ICommand> ContentsCommandFactory::createGetCommand(const HttpRequest& a_request) const
        { 
            if (a_request.m_routing_path.size() == 0)
            {
                //GET /content
                return std::make_unique<GetContentsCommand>(a_request.m_query_params, m_contents_contract, m_content_mapper, m_visitor_factory);
            }

            if (a_request.m_routing_path.size() == 1 && m_guid_parser->isGuid(a_request.m_routing_path.at(0)))
            {
               //GET /content/{contentId}
               auto contentId = m_guid_parser->parseGuid(a_request.m_routing_path.at(0));
               return std::make_unique<GetContentByIdCommand>(contentId, m_contents_contract, m_content_mapper, m_visitor_factory); 
            }

            if (a_request.m_routing_path.size() == 2 && a_request.m_routing_path.at(0) == AUTHOR_ROUTING_PATH_VALUE && m_guid_parser->isGuid(a_request.m_routing_path.at(1)))
            {
                //GET /content/author/{ownerId}
                auto contentId = m_guid_parser->parseGuid(a_request.m_routing_path.at(0));//move logic to command?
                return std::make_unique<GetContentsByOwnerIdCommand>(contentId, m_contents_contract, m_content_mapper, m_visitor_factory);
            }

            return nullptr; 
        }

        std::unique_ptr<ICommand> ContentsCommandFactory::createPostCommand(const HttpRequest& a_request) const
        { 
            if (a_request.m_routing_path.size() == 0)
            {
                //POST /content
                return std::make_unique<AddContentCommand>(a_request.m_body, m_contents_contract, m_content_mapper, m_visitor_factory);
            }

            return nullptr;
        }

        std::unique_ptr<ICommand> ContentsCommandFactory::createPutCommand(const HttpRequest& a_request) const 
        { 
            if (a_request.m_routing_path.size() == 1 && m_guid_parser->isGuid(a_request.m_routing_path.at(0)))
            {
                //PUT /content/{contentId}
                auto contentId = m_guid_parser->parseGuid(a_request.m_routing_path.at(0));
                return std::make_unique<UpdateContentCommand>(contentId, a_request.m_body, m_contents_contract, m_content_mapper, m_visitor_factory);
            }

            return nullptr; 
        }

        std::unique_ptr<ICommand> ContentsCommandFactory::createDeleteCommand(const HttpRequest& a_request) const 
        { 
            if (a_request.m_routing_path.size() == 1 && m_guid_parser->isGuid(a_request.m_routing_path.at(0)))
            {
                //DELETE /content/{contentId}
                auto contentId = m_guid_parser->parseGuid(a_request.m_routing_path.at(0));
                return std::make_unique<DeleteContentCommand>(contentId, m_contents_contract);
            }

            return nullptr; 
        } 

        std::unique_ptr<ICommand> ContentsCommandFactory::createPatchCommand(const HttpRequest& a_request) const { return nullptr; }
    }
}