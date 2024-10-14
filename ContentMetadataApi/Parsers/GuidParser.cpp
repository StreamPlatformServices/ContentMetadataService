#pragma once
#include "GuidParser.hpp"

namespace ContentMetadataApi
{
	namespace Parsers
	{
        bool GuidParser::isGuid(const boost::uuids::uuid& uuid) const
        {
            if (uuid.is_nil())
            {
                return false;
            }

            try
            {
                boost::uuids::uuid parsed_uuid = boost::uuids::string_generator()(to_string(uuid));
                return !parsed_uuid.is_nil();
            }
            catch (const std::exception& e)
            {
                return false;
            }
        }

        bool GuidParser::isGuid(const std::string& a_guid_string) const
        {
            try
            {
                boost::uuids::string_generator gen;
                boost::uuids::uuid u = gen(a_guid_string);
                return true;
            }
            catch (const std::exception& e)
            {
                return false;
            }
        }

        auto GuidParser::parseGuid(const std::string& a_guid_string) const -> boost::uuids::uuid
        {
            try
            {
                boost::uuids::string_generator gen;
                return gen(a_guid_string);
            }
            catch (const std::exception& e)
            {
                std::cerr << "Wrong guid format exception: " << e.what() << std::endl;
                throw;
            }
        }
	}
}