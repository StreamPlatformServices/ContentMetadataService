#include "../Entities/Content.h"
#include "../Awaitable.h"

//CONTENT_METADATA_CORE_EXPORT void TestFunction();

namespace ContentMetadataCore
{
    namespace InternalContracts
    {
        //std::async with std::future creates new threads so better to use std::couritine for simple async I/O operations
        /*class CONTENT_METADATA_CORE_EXPORT IContentsContract
        {
        public:
            virtual ~IContentMetadataContract() = default;
            virtual std::future<std::vector<Content>> GetAllContentsAsync(int limit, int offset) = 0;
            virtual std::future<Content> GetContentMetadataByIdAsync(boost::uuids::uuid contentId) = 0;
            virtual std::future<std::vector<Content>> GetContentMetadataByOwnerIdAsync(boost::uuids::uuid ownerId) = 0;
            virtual std::future<void> EditContentMetadataAsync(boost::uuids::uuid contentId, Content content) = 0;
            virtual std::future<boost::uuids::uuid> AddContentMetadataAsync(Content content) = 0;
            virtual std::future<void> DeleteContentMetadataAsync(boost::uuids::uuid contentId) = 0;
        };*/

        class CONTENT_METADATA_CORE_EXPORT IContentsContract
        {
        public:
            virtual ~IContentsContract() = default;

            virtual Awaitable<std::vector<Entities::Content>> GetAllContentsAsync(int limit, int offset) = 0;
            virtual Awaitable<Entities::Content> GetContentMetadataByIdAsync(boost::uuids::uuid contentId) = 0;
            virtual Awaitable<std::vector<Entities::Content>> GetContentMetadataByOwnerIdAsync(boost::uuids::uuid ownerId) = 0;
            virtual Awaitable<void> EditContentMetadataAsync(boost::uuids::uuid contentId, Entities::Content content) = 0;
            virtual Awaitable<boost::uuids::uuid> AddContentMetadataAsync(Entities::Content content) = 0;
            virtual Awaitable<void> DeleteContentMetadataAsync(boost::uuids::uuid contentId) = 0;
        };

    }
}