#pragma once

#include "ContentDto.hpp"

namespace ContentMetadataApi
{
    namespace Dto
    {
        struct ContentsDto : public Visitors::IVisitable
        {
            std::vector<ContentDto> m_contents;

            void accept(Visitors::IVisitor& a_visitor) override
            {
                a_visitor.visit(*this);
            }
        };
    }
}