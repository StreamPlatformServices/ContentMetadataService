#pragma once

namespace ContentMetadataApi
{
    enum class HttpMethod 
    {
        Get,
        Post,
        Put,
        Delete,
        Patch,
        Head,
        Options,
        Connect,
        Trace,
        Unknown 
    };
}