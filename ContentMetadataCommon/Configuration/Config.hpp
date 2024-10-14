#include "IConfig.hpp"

namespace ContentMetadataCommon
{
    class Config : public IConfig
    {
    public:
        explicit Config(const std::string& filename, bool a_use_test_db = false);

        void printConfig() const noexcept;

        auto getDbUsername() const noexcept -> std::string override;
        auto getDbPassword() const noexcept -> std::string override;
        auto getDbHost() const noexcept -> std::string override;
        auto getDbName() const noexcept -> std::string override;
        int getDbPort() const noexcept override;

        int getServerPort() const noexcept override;
        int getThreadPoolLimit() const noexcept override;

    private:
        std::string m_db_username;
        std::string m_db_password;
        std::string m_db_host;
        int m_db_port;
        std::string m_db_name;

        int m_server_port;
        int m_thread_pool_limit;
    };
}