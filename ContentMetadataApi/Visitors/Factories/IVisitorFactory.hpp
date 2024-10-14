#pragma once
#include "../../StdAfx.hpp"
#include "../IVisitor.hpp"

namespace ContentMetadataApi
{
	namespace Visitors
	{
		class IVisitorFactory
		{
		public:
			virtual ~IVisitorFactory() = default;

			virtual auto createDataValidationVisitor(std::vector<std::string>& a_errors_output) const -> std::unique_ptr<IVisitor> = 0;
			virtual auto createJsonDeserializationVisitor(const std::string& a_json_string_input) const -> std::unique_ptr<IVisitor> = 0;
			virtual auto createJsonSerializationVisitor(std::string& a_json_string_output) const -> std::unique_ptr<IVisitor> = 0;

		};
	}
}