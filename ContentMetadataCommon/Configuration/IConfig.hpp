#pragma once
#include <iostream>
#include <string>
#include <mINI/ini.h>

namespace ContentMetadataCommon
{
    class IConfig
    {
    public:
        virtual std::string getDbUsername() const noexcept = 0;
        virtual std::string getDbPassword() const noexcept = 0;
        virtual std::string getDbHost() const noexcept = 0;
        virtual int getDbPort() const noexcept = 0;
        virtual std::string getDbName() const noexcept = 0;
        
        virtual int getServerPort() const noexcept = 0;
        virtual int getThreadPoolLimit() const noexcept = 0;

        //virtual std::string getLogLevel() const noexcept = 0; //TODO:
    };
}