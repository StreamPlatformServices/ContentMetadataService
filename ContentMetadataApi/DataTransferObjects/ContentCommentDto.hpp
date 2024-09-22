#pragma once

#include "../StdAfx.hpp"
#include "../Visitors/IVisitable.hpp"

namespace ContentMetadataApi
{
    namespace Dto
    {
        struct ContentCommentDto : public Visitors::IVisitable
        {
            boost::uuids::uuid m_uuid = boost::uuids::nil_uuid();
            std::string m_body;
            std::chrono::system_clock::time_point m_creation_time = std::chrono::system_clock::now();
            boost::uuids::uuid m_content_id = boost::uuids::nil_uuid();
            std::string m_user_name;

            void accept(Visitors::IVisitor& a_visitor) override
            {
                a_visitor.visit(*this);
            }
        };
    }
}