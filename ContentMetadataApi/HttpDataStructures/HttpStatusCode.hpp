#pragma once

namespace ContentMetadataApi
{
    enum class HttpStatusCode 
    {
        NotFound = 404,
        BadRequest = 400,
        InternalServerError = 500,
        Ok = 200
    };
}