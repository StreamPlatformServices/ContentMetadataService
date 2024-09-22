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

			virtual std::unique_ptr<IVisitor> createDataValidationVisitor(std::vector<std::string>& a_errors_output) const = 0;
			virtual std::unique_ptr<IVisitor> createJsonDeserializationVisitor(const std::string& a_json_string_input) const = 0;
			virtual std::unique_ptr<IVisitor> createJsonSerializationVisitor(std::string& a_json_string_output) const = 0;

		};
	}
}