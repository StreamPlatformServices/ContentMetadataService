#pragma once
#include <gmock/gmock.h>
#include "../../ContentMetadataApi/Parsers/IGuidParser.hpp"
#include "../../ContentMetadataApi/Parsers/GuidParser.hpp"

namespace ContentMetadataApi_Tests 
{
    namespace Mocks 
    {
        class GuidParserMock : public ContentMetadataApi::Parsers::IGuidParser 
        {
        public:
            MOCK_METHOD(bool, isGuid, (const boost::uuids::uuid& uuid), (const, override));
            MOCK_METHOD(bool, isGuid, (const std::string& a_guid_string), (const, override));
            MOCK_METHOD(boost::uuids::uuid, parseGuid, (const std::string& a_guid_string), (const, override));


            std::unique_ptr<ContentMetadataApi::Parsers::GuidParser> real_guid_parser;

            GuidParserMock()
            {
                real_guid_parser = std::make_unique<ContentMetadataApi::Parsers::GuidParser>();
            }

            void delegateToReal()
            {
                ON_CALL(*this, isGuid(testing::An<const boost::uuids::uuid&>()))
                    .WillByDefault([this](const boost::uuids::uuid& uuid)
                        {
                            return real_guid_parser->isGuid(uuid);
                        });

                ON_CALL(*this, isGuid(testing::An<const std::string&>()))
                    .WillByDefault([this](const std::string& a_guid_string)
                        {
                            return real_guid_parser->isGuid(a_guid_string);
                        });

                ON_CALL(*this, parseGuid(testing::_))
                    .WillByDefault([this](const std::string& a_guid_string)
                        {
                            return real_guid_parser->parseGuid(a_guid_string);
                        });
            }
        };
    }
}
