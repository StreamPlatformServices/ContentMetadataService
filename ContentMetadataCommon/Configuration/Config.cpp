#include "Config.hpp"
#include <thread>
#include <iostream>
#include <algorithm>

namespace
{
    constexpr auto DATABASE_SECTION = "database";
    constexpr auto TEST_DATABASE_SECTION = "test_database";
    constexpr auto SERVER_SECTION = "server";
    constexpr auto GENERAL_SECTION = "general";

    constexpr auto DB_NAME_FIELD = "dbname";
    constexpr auto USER_NAME_FIELD = "username";
    constexpr auto PASSWORD_FIELD = "password";
    constexpr auto HOST_FIELD = "host";
    constexpr auto PORT_FIELD = "port";
    constexpr auto THREADS_LIMIT_FIELD = "threads_limit";
    
    constexpr auto DB_NAME_DEFAULT_VALUE = "ContentMetadata";
    constexpr auto DB_USER_NAME_DEFAULT_VALUE = "postgres";
    constexpr auto DB_PASSWORD_DEFAULT_VALUE = "qwerty123456";
    constexpr auto DB_HOST_DEFAULT_VALUE = "localhost";
    constexpr auto DB_PORT_DEFAULT_VALUE = 5432;
    constexpr auto SERVER_PORT_DEFAULT_VALUE = 5050;
    constexpr auto THREADS_LIMIT_PROC_CORES_MULTIPLICATION = 2u;

    constexpr auto DB_TEST_NAME_DEFAULT_VALUE = "ContentMetadataTest";

    constexpr auto USE_TEST_DB_FIELD = "use_test_db";
}

namespace ContentMetadataCommon
{
    Config::Config(const std::string& a_filename, bool a_use_test_db)
    {
        mINI::INIFile file(a_filename);
        mINI::INIStructure ini;
        file.read(ini);

        const std::string& db_section = a_use_test_db ? TEST_DATABASE_SECTION : DATABASE_SECTION;

        //[database or test_database]
        m_db_username = ini[db_section][USER_NAME_FIELD].empty()
            ? DB_USER_NAME_DEFAULT_VALUE
            : ini[db_section][USER_NAME_FIELD];

        m_db_password = ini[db_section][PASSWORD_FIELD].empty()
            ? DB_PASSWORD_DEFAULT_VALUE
            : ini[db_section][PASSWORD_FIELD];

        m_db_host = ini[db_section][HOST_FIELD].empty()
            ? DB_HOST_DEFAULT_VALUE
            : ini[db_section][HOST_FIELD];

        m_db_port = ini[db_section][PORT_FIELD].empty()
            ? DB_PORT_DEFAULT_VALUE
            : std::stoi(ini[db_section][PORT_FIELD]);

        m_db_name = ini[db_section][DB_NAME_FIELD].empty()
            ? (a_use_test_db ? DB_TEST_NAME_DEFAULT_VALUE : DB_NAME_DEFAULT_VALUE)
            : ini[db_section][DB_NAME_FIELD];

        //[server]
        m_server_port = ini[SERVER_SECTION][PORT_FIELD].empty()
            ? SERVER_PORT_DEFAULT_VALUE
            : std::stoi(ini[SERVER_SECTION][PORT_FIELD]);

        auto thread_limit_default_value = std::max(THREADS_LIMIT_PROC_CORES_MULTIPLICATION,
            std::thread::hardware_concurrency() * THREADS_LIMIT_PROC_CORES_MULTIPLICATION);

        m_thread_pool_limit = ini[SERVER_SECTION][THREADS_LIMIT_FIELD].empty()
            ? thread_limit_default_value
            : std::stoi(ini[SERVER_SECTION][THREADS_LIMIT_FIELD]);

        //log_level = ini[SERVER_SECTION]["log_level"].empty() ? "info" : ini[SERVER_SECTION]["log_level"]; //TODO:
    }

    void Config::printConfig() const noexcept
    {
        std::cout << "[Database Config]" << std::endl;
        std::cout << "Username: " << m_db_username << std::endl;
        std::cout << "Password: " << m_db_password << std::endl;
        std::cout << "Host: " << m_db_host << std::endl;
        std::cout << "Port: " << m_db_port << std::endl;
        std::cout << "DB Name: " << m_db_name << std::endl;

        std::cout << "\n[Server Config]" << std::endl;
        std::cout << "Server Port: " << m_server_port << std::endl;
        std::cout << "Maximal threads in pool: " << m_thread_pool_limit << std::endl;

        //std::cout << "Log Level: " << log_level << std::endl; //TODO:
    }

    std::string Config::getDbUsername() const noexcept { return m_db_username; }
    std::string Config::getDbPassword() const noexcept { return m_db_password; }
    std::string Config::getDbHost() const noexcept { return m_db_host; }
    int Config::getDbPort() const noexcept { return m_db_port; }
    std::string Config::getDbName() const noexcept { return m_db_name; }

    int Config::getServerPort() const noexcept { return m_server_port; }
    int Config::getThreadPoolLimit() const noexcept { return m_thread_pool_limit; }

    //std::string Config::getLogLevel() const noexcept { return log_level; } //TODO:
}
