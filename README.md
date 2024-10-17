# ContentMetadataService

ContentMetadataService provides a way to manage metadata for various types of content and is a critical component of a larger streaming platform system. It interacts with a PostgreSQL database to store and retrieve metadata efficiently. The service can be built using CMake and supports both Linux/macOS and Windows platforms. It also includes a configuration system that allows you to adjust database connections and server settings through a `config.ini` file.

This README will guide you through the process of cloning the repository, building the project, configuring PostgreSQL, and setting up the necessary environment to run the service.

For more details on the structure and functionality of the service, please refer to the accompanying [documentation](link-to-documentation), which explains the architecture and key components in depth.

## Table of Contents

- [Requirements](#requirements)
- [Cloning the repository](#cloning-the-repository)
- [Building the project](#building-the-project)
  - [Windows (Visual Studio)](#windows-visual-studio)
  - [Linux/macOS](#linuxmacos)
- [PostgreSQL Configuration](#postgresql-configuration)
- [Configuring the `config.ini` file](#configuring-the-configini-file)
- [Running Content Metadata Service](#running-content-metadata-service)
- [Running Components Unit Tests](#running-components-unit-tests)


## Requirements

Before starting the installation, ensure you have the following tools installed:

- **Git**: [Installation Guide](https://git-scm.com/downloads)
- **CMake**: [Installation Guide](https://cmake.org/download/)
- **Compiler**: Needs to support C++20, such as: GCC 10 or later, Clang 10 or later, MSVC from Visual Studio 2019 version 16.8 or later
- **PostgreSQL**: [PostgreSQL Installation Guide](https://www.postgresql.org/download/)


## Cloning the repository

The repository contains submodules that also need to be fetched. To clone the repository along with submodules, run the following command:

```
git clone --recurse-submodules https://github.com/StreamPlatformServices/ContentMetadataService.git
```

If you have cloned the repository without submodules, you can update them later with:

```
git submodule update --init --recursive
```

## Building the project

### Windows / Linux 

1. Open a terminal and navigate to the project directory:

    ```
    cd ContentMetadataService
    ```

2. Create a build directory:

    ```
    mkdir build
    cd build
    ```

3. Generate the build files using CMake:

    ```
    cmake ..
    ```

4. Build the project:

    ```
   cmake --build . --config Release --parallel <number of threads>
    ```
    
5. **(Windows only)** Install the project (this will move all necessary files, such as DLLs, to the correct directory):

    ```
    cmake --install . --config Release
    ```

> **Note**: The `install` step is necessary only on Windows to move DLL files to the same directory as the executable. On Linux, this step can be skipped.

### Notes:
- Make sure that your compiler (e.g. GCC, Clang, or MSVC) is properly installed and available in your system's PATH.
- CMake will automatically detect and use the default compiler on your system. You can specify a particular compiler using the `-DCMAKE_C_COMPILER` and `-DCMAKE_CXX_COMPILER` options if necessary.

## PostgreSQL Configuration

### Installing PostgreSQL

- **Linux (Ubuntu)**:

   ```
   sudo apt update
   sudo apt install postgresql postgresql-contrib libpq-dev
   ```

- **Windows**: You can download PostgreSQL from the [official site](https://www.postgresql.org/download/windows/).

### Checking if PostgreSQL is running

- Make sure the PostgreSQL server is running:

- **Linux**:

   ```
   sudo systemctl status postgresql
   sudo systemctl start postgresql
   ```

- **Windows**: PostgreSQL should automatically start after installation.

### Setting Environment Variables

Ensure that the path to PostgreSQL is set in your environment variables so that PostgreSQL binaries, such as `psql`, are accessible from the command line. This is particularly important on Windows. You can verify or add PostgreSQL to the `PATH` environment variable:

- **Linux**: Paths are usually configured automatically, but if needed, you can add them manually e.g.:

   ```
   export PATH="path/to/pgSql:$PATH"
   ```

- **Windows**: Add the path to the PostgreSQL binaries to the `Path` environment variable in the system settings.

### Creating the Database

To create a new database, log into PostgreSQL and run the following command:

```
sudo -u postgres psql
CREATE DATABASE ContentMetadata;
```

## Configuring the `config.ini` file

Before running the project, you need to configure the connection to the database in the `config.ini` file (It should be created in executable directory). Here is an example of the file:

```
[database]
dbname = ContentMetadata
username = postgres
password = secret_password
host = localhost
port = 5432

[server]
port = 8080
threads_limit = 16
```

- **[database]**: The main project database configuration.
- **[server]**: Application server settings such as port and thread limit.

If any fields in the `config.ini` file are omitted, default values will be used.

## Running Content Metadata Service

After correctly configuring the project and the database settings in `config.ini`, you can run:
- **Windows**:
    ```
    ContentMetadataService.exe file. 
    ```
- **Linux**:
    ```
    ./ContentMetadataService. 
    ```
(Make sure that the PostgreSQL server is running and accessible.)

## Running Components Unit Tests

Before running the unit tests for each component, create a separate databse in PgSql for testing purpouse. Ensure that the separate `config.ini` file is properly configured for each test component such as **ContentMetadataRepository_Tests**, **ContentMetadataApi_Tests**, and **ContentMetadataCommon_Tests**. Make sure that a test database is set up and accessible.

To build and run the unit tests for each component of the project, follow these steps:

### 1. ContentMetadataApi Tests

Navigate to the test directory and run the tests:

```
cd tests/ContentMetadataApi_Tests
```

- **Windows**: Install the component to ensure all necessary files (such as DLLs) are in the correct directory:

    ```
    cmake --install . --config Release
    ContentMetadataApi_Tests.exe
    ```

- **Linux**: Simply run the test executable:

    ```
    ./ContentMetadataApi_Tests
    ```

Return to the project root directory:

```
cd ../..
```

### 2. ContentMetadataCommon Tests

Navigate to the test directory and run the tests:

```
cd tests/ContentMetadataCommon_Tests
```

- **Windows**: Install the component:

    ```
    cmake --install . --config Release
    ContentMetadataCommon_Tests.exe
    ```

- **Linux**: Run the test executable:

    ```
    ./ContentMetadataCommon_Tests
    ```

Return to the project root directory:

```
cd ../..
```

### 3. ContentMetadataRepository Tests

Navigate to the test directory and run the tests:

```
cd tests/ContentMetadataRepository_Tests
```

- **Windows**: Install the component:

    ```
    cmake --install . --config Release
    ContentMetadataRepository_Tests.exe
    ```

- **Linux**: Run the test executable:

    ```
    ./ContentMetadataRepository_Tests
    ```

Return to the project root directory:

```
cd ../..
```
