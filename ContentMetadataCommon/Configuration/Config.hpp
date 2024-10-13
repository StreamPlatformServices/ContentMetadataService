#include "IConfig.hpp"

namespace ContentMetadataCommon
{
    class Config : public IConfig
    {
    public:
        explicit Config(const std::string& filename, bool a_use_test_db = false);

        void printConfig() const noexcept;

        std::string getDbUsername() const noexcept override;
        std::string getDbPassword() const noexcept override;
        std::string getDbHost() const noexcept override;
        int getDbPort() const noexcept override;
        std::string getDbName() const noexcept override;

        int getServerPort() const noexcept override;
        int getThreadPoolLimit() const noexcept override;

        //std::string getLogLevel() const noexcept; //TODO:

    private:
        std::string m_db_username;
        std::string m_db_password;
        std::string m_db_host;
        int m_db_port;
        std::string m_db_name;

        int m_server_port;
        int m_thread_pool_limit;
        
        //std::string m_log_level; //TODO: 
    };
}