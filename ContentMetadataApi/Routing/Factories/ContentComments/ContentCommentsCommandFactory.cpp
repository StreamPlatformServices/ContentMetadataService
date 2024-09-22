#include "ContentCommentsCommandFactory.hpp"
#include "../../Commands/ContentComments/AddContentCommentCommand.hpp"
#include "../../Commands/ContentComments/DeleteContentCommentCommand.hpp"

namespace ContentMetadataApi
{
    namespace Routing
    {
        ContentCommentsCommandFactory::ContentCommentsCommandFactory(
            std::shared_ptr<ContentMetadataCore::IContentCommentsContract> a_content_comments_contract,
            std::shared_ptr<Mappers::IContentCommentDataMapper> a_content_comment_mapper,
            std::shared_ptr<Parsers::IGuidParser> a_guid_parser,
            std::shared_ptr<Visitors::IVisitorFactory>	a_visitor_factory) noexcept
            : m_content_comments_contract(a_content_comments_contract)
            , m_content_comment_mapper(a_content_comment_mapper)
            , m_guid_parser(a_guid_parser)
            , m_visitor_factory(a_visitor_factory)
        {
        }

        std::unique_ptr<ICommand> ContentCommentsCommandFactory::createGetCommand(const HttpRequest& a_request) const
        { 
            return nullptr; 
        }

        std::unique_ptr<ICommand> ContentCommentsCommandFactory::createPostCommand(const HttpRequest& a_request) const 
        { 
            if (a_request.m_routing_path.size() == 0)
            {
                //POST /comment
                return std::make_unique<AddContentCommentCommand>(a_request.m_body, m_content_comments_contract, m_content_comment_mapper, m_visitor_factory);
            }

            return nullptr;
        }

        std::unique_ptr<ICommand> ContentCommentsCommandFactory::createPutCommand(const HttpRequest& a_request) const { return nullptr; }

        std::unique_ptr<ICommand> ContentCommentsCommandFactory::createDeleteCommand(const HttpRequest& a_request) const 
        { 
            if (a_request.m_routing_path.size() == 1 && m_guid_parser->isGuid(a_request.m_routing_path.at(0)))
            {
                //DELETE /comment/{contentCommentId}
                auto contentId = m_guid_parser->parseGuid(a_request.m_routing_path.at(0));
                return std::make_unique<DeleteContentCommentCommand>(contentId, m_content_comments_contract);
            }

            return nullptr; 
        }

        std::unique_ptr<ICommand> ContentCommentsCommandFactory::createPatchCommand(const HttpRequest& a_request) const { return nullptr; }
    }
}