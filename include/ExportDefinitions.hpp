#pragma once

#ifdef BUILD_CONTENT_METADATA_API
#define CONTENT_METADATA_API_EXPORT __declspec(dllexport)
#else
#define CONTENT_METADATA_API_EXPORT __declspec(dllimport)
#endif

#ifdef BUILD_CONTENT_METADATA_CORE
#define CONTENT_METADATA_CORE_EXPORT __declspec(dllexport)
#else
#define CONTENT_METADATA_CORE_EXPORT __declspec(dllimport)
#endif

#ifdef BUILD_CONTENT_METADATA_REPOSITORY
#define CONTENT_METADATA_REPOSITORY_EXPORT __declspec(dllexport)
#else
#define CONTENT_METADATA_REPOSITORY_EXPORT __declspec(dllimport)
#endif
