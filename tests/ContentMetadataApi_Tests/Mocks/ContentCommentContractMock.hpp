#pragma once
#include <gmock/gmock.h>
#include "../../ContentMetadataCore/InternalContracts/IContentCommentsContract.hpp"

namespace ContentMetadataApi_Tests 
{
    namespace Mocks 
    {
        class ContentCommentContractMock : public ContentMetadataCore::IContentCommentsContract
        {
        public:
            MOCK_METHOD(boost::asio::awaitable<boost::uuids::uuid>, addContentCommentAsync, (ContentMetadataCore::Entities::ContentComment a_content_comment), (override));
            MOCK_METHOD(boost::asio::awaitable<void>, deleteContentCommentAsync, (boost::uuids::uuid a_content_comment_id), (override));
        };
    }
}
