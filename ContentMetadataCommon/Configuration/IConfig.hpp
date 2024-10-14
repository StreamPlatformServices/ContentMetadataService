#pragma once
#include <iostream>
#include <string>
#include <mINI/ini.h>

namespace ContentMetadataCommon
{
    class IConfig
    {
    public:
        virtual auto getDbUsername() const noexcept -> std::string = 0;
        virtual auto getDbPassword() const noexcept -> std::string = 0;
        virtual auto getDbHost() const noexcept -> std::string = 0;
        virtual auto getDbName() const noexcept -> std::string = 0;
        virtual int getDbPort() const noexcept = 0;
        
        virtual int getServerPort() const noexcept = 0;
        virtual int getThreadPoolLimit() const noexcept = 0;

        //virtual std::string getLogLevel() const noexcept = 0; //TODO:
    };
}