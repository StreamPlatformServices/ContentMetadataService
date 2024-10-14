#include "HttpCommandFactoriesFactory.hpp"
#include "ContentComments/ContentCommentsCommandFactory.hpp"
#include "Contents/ContentsCommandFactory.hpp"

namespace
{
    constexpr auto CONTENTS_ENDPOINT = "content";
    constexpr auto CONTENT_COMMENTS_ENDPOINT = "comment";
};

namespace ContentMetadataApi
{
    namespace Routing
    {
        HttpCommandFactoriesFactory::HttpCommandFactoriesFactory(
            std::shared_ptr<ContentMetadataCore::IContentsContract>        a_contents_contract,
            std::shared_ptr<ContentMetadataCore::IContentCommentsContract> a_content_comments_contract,
            std::shared_ptr<Mappers::IContentDataMapper>				   a_content_mapper,
            std::shared_ptr<Mappers::IContentCommentDataMapper>			   a_content_comment_mapper,
            std::shared_ptr<Parsers::IGuidParser>						   a_guid_parser,
            std::shared_ptr<Visitors::IVisitorFactory>					   a_visitor_factory) noexcept
            : m_contents_contract(a_contents_contract)
            , m_content_comments_contract(a_content_comments_contract)
            , m_content_mapper(a_content_mapper)
            , m_content_comment_mapper(a_content_comment_mapper)
            , m_guid_parser(a_guid_parser)
            , m_visitor_factory(a_visitor_factory)
        {
        }

        auto HttpCommandFactoriesFactory::createHttpCommandFactory(const std::string& a_root_endpoint) const -> std::unique_ptr<IHttpCommandFactory>
        {
            if (a_root_endpoint == CONTENT_COMMENTS_ENDPOINT)
            {
                return std::make_unique<ContentCommentsCommandFactory>(m_content_comments_contract, m_content_comment_mapper, m_guid_parser, m_visitor_factory);
            }

            if (a_root_endpoint == CONTENTS_ENDPOINT)
            {
                return std::make_unique<ContentsCommandFactory>(m_contents_contract, m_content_mapper, m_guid_parser, m_visitor_factory);
            }

            return nullptr;
        }
    }
}