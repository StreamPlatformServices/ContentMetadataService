#pragma once
#include "../StdAfx.hpp"
#include "../HttpDataStructures/HttpRequest.hpp"
#include "../HttpDataStructures/HttpResponse.hpp"

namespace ContentMetadataApi
{
	namespace Mappers
	{
		
		class IHttpDataMapper
		{
		public:
			virtual ~IHttpDataMapper() = default;

			virtual auto httpRequestFrom(const boost::beast::http::request<boost::beast::http::string_body>& a_request) const -> HttpRequest = 0;
			virtual auto BoostBeastResponseFrom(const HttpResponse& a_response) const -> boost::beast::http::response<boost::beast::http::string_body> = 0;
		};
	}
}