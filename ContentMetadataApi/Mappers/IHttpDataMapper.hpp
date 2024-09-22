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

			virtual HttpRequest httpRequestFrom(const boost::beast::http::request<boost::beast::http::string_body>& a_request) const = 0;
			virtual boost::beast::http::response<boost::beast::http::string_body> BoostBeastResponseFrom(const HttpResponse& a_response) const = 0;

		};
	}
}