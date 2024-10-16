#include "Config.hpp"
#include <thread>
#include <iostream>
#include <algorithm>

namespace
{
    constexpr auto DATABASE_SECTION = "database";
    constexpr auto SERVER_SECTION = "server";

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
}

namespace ContentMetadataCommon
{
    Config::Config(const std::string& a_filename)
    {
        mINI::INIFile file(a_filename);
        mINI::INIStructure ini;
        file.read(ini);

        //[database]
        m_db_username = ini[DATABASE_SECTION][USER_NAME_FIELD].empty()
            ? DB_USER_NAME_DEFAULT_VALUE
            : ini[DATABASE_SECTION][USER_NAME_FIELD];

        m_db_password = ini[DATABASE_SECTION][PASSWORD_FIELD].empty()
            ? DB_PASSWORD_DEFAULT_VALUE
            : ini[DATABASE_SECTION][PASSWORD_FIELD];

        m_db_host = ini[DATABASE_SECTION][HOST_FIELD].empty()
            ? DB_HOST_DEFAULT_VALUE
            : ini[DATABASE_SECTION][HOST_FIELD];

        m_db_port = ini[DATABASE_SECTION][PORT_FIELD].empty()
            ? DB_PORT_DEFAULT_VALUE
            : std::stoi(ini[DATABASE_SECTION][PORT_FIELD]);

        m_db_name = ini[DATABASE_SECTION][DB_NAME_FIELD].empty()
            ? DB_NAME_DEFAULT_VALUE
            : ini[DATABASE_SECTION][DB_NAME_FIELD];

        //[server]
        m_server_port = ini[SERVER_SECTION][PORT_FIELD].empty()
            ? SERVER_PORT_DEFAULT_VALUE
            : std::stoi(ini[SERVER_SECTION][PORT_FIELD]);

        auto thread_limit_default_value = std::max(THREADS_LIMIT_PROC_CORES_MULTIPLICATION,
            std::thread::hardware_concurrency() * THREADS_LIMIT_PROC_CORES_MULTIPLICATION);

        m_thread_pool_limit = ini[SERVER_SECTION][THREADS_LIMIT_FIELD].empty()
            ? thread_limit_default_value
            : std::stoi(ini[SERVER_SECTION][THREADS_LIMIT_FIELD]);
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
        std::cout << "Maximal threads in pool: " << m_thread_pool_limit << '\n' << std::endl;
    }

    auto Config::getDbUsername() const noexcept  -> std::string { return m_db_username; }
    auto Config::getDbPassword() const noexcept -> std::string { return m_db_password; }
    auto Config::getDbHost() const noexcept -> std::string { return m_db_host; }
    auto Config::getDbName() const noexcept -> std::string { return m_db_name; }
    int Config::getDbPort() const noexcept { return m_db_port; }

    int Config::getServerPort() const noexcept { return m_server_port; }
    int Config::getThreadPoolLimit() const noexcept { return m_thread_pool_limit; }
}
