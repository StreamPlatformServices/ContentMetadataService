#pragma once
#include <gmock/gmock.h>
#include "../../../ContentMetadataApi/Visitors/IVisitor.hpp"

namespace ContentMetadataApi_Tests
{
    namespace Mocks
    {
        class VisitorMock : public ContentMetadataApi::Visitors::IVisitor
        {
        public:
            MOCK_METHOD(void, visit, (ContentMetadataApi::Dto::ContentDto& a_visitable), (override));
            MOCK_METHOD(void, visit, (ContentMetadataApi::Dto::ContentCommentDto& a_visitable), (override));
            MOCK_METHOD(void, visit, (ContentMetadataApi::Dto::LicenseRulesDto& a_visitable), (override));
            MOCK_METHOD(void, visit, (ContentMetadataApi::Dto::GuidDto& a_visitable), (override));
            MOCK_METHOD(void, visit, (ContentMetadataApi::Dto::ContentsDto& a_visitable), (override));
            MOCK_METHOD(void, visit, (ContentMetadataApi::Dto::ErrorsDto& a_visitable), (override));
        };
    }
}