#pragma once
#include "IHttpDataMapper.hpp"
#include "../Parsers/IEndpointParser.hpp"


namespace ContentMetadataApi
{
	namespace Mappers
	{
		class CONTENT_METADATA_API_EXPORT HttpDataMapper : public IHttpDataMapper
		{
		public:
			explicit HttpDataMapper(std::shared_ptr<Parsers::IEndpointParser> a_endpoint_parser) noexcept;

			HttpDataMapper(const HttpDataMapper&) = delete;
			HttpDataMapper(HttpDataMapper&&) = delete;

			HttpDataMapper& operator=(const HttpDataMapper&) = delete;
			HttpDataMapper& operator=(HttpDataMapper&&) = delete;

			HttpRequest httpRequestFrom(const boost::beast::http::request<boost::beast::http::string_body>& a_request) const override;
			boost::beast::http::response<boost::beast::http::string_body> BoostBeastResponseFrom(const HttpResponse& a_response) const override;

		private:
			HttpMethod mapMethod(boost::beast::http::verb a_method) const;
			std::unordered_map<std::string, std::string> mapHeaders(const boost::beast::http::request<boost::beast::http::string_body>& a_request) const;

			std::shared_ptr<Parsers::IEndpointParser> m_endpoint_parser;
		};
	}
}