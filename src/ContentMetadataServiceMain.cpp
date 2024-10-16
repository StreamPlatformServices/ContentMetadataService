#include "ExportDefinitions.hpp"

#include "../ContentMetadataCommon/Configuration/Config.hpp"
#include "../ContentMetadataCommon/Configuration/IConfig.hpp"
#include "../ContentMetadataCommon/Threads/IThreadPoolManager.hpp"
#include "../ContentMetadataCommon/Threads/ThreadPoolManager.hpp"

#include "../ContentMetadataRepository/Persistance/DatabaseSchema/DatabaseSchemaInitializer.hpp"
#include "../ContentMetadataRepository/Persistance/Repositories/RepositoryFactory.hpp"
#include "../ContentMetadataRepository/Persistance/UnitOfWork.hpp"
#include "../ContentMetadataRepository/ContentCommentsDatabaseService.hpp"
#include "../ContentMetadataRepository/ContentsDatabaseService.hpp"

#include "../ContentMetadataApi/Mappers/EntityMappers/ContentCommentDataMapper.hpp"
#include "../ContentMetadataApi/Mappers/EntityMappers/LicenseRulesDataMapper.hpp"
#include "../ContentMetadataApi/Mappers/EntityMappers/ContentDataMapper.hpp"
#include "../ContentMetadataApi/Routing/Factories/HttpCommandFactoriesFactory.hpp"
#include "../ContentMetadataApi/Routing/CommandSelector.hpp"
#include "../ContentMetadataApi/Parsers/EndpointParser.hpp"
#include "../ContentMetadataCommon/DateTime/DateTimeParser.hpp"
#include "../ContentMetadataApi/Parsers/GuidParser.hpp"
#include "../ContentMetadataApi/Mappers/HttpDataMapper.hpp"
#include "../ContentMetadataApi/Visitors/Factories/VisitorFactory.hpp"
#include "../ContentMetadataApi/HttpServer.hpp"

/*
* TODO:
* 1. Change pqxx library to pqlib, use async couritine, remove redundant thread pool, and add pg sql connections limit in configuration
* 2. Use async spdlog for logging
* 3. Use different content's dto's for response and request (there is no use for comments in content stucture while adding content)
* 4. Clear warnings.
*/

namespace
{
	constexpr auto CONFIGURATION_FILE_NAME = "config.ini";
}

int main()
{
	auto configuration = std::make_shared<ContentMetadataCommon::Config>(CONFIGURATION_FILE_NAME);
	configuration->printConfig();

	auto thread_pool_manager = std::make_shared<ContentMetadataCommon::ThreadPoolManager>(configuration->getThreadPoolLimit());
	auto date_time_parser = std::make_shared<ContentMetadataCommon::DateTimeParser>();

	//ContentMetadataRepository
	{
		ContentMetadataRepository::DatabaseSchemaInitializer database_schema_manager(std::static_pointer_cast<ContentMetadataCommon::IConfig>(configuration));
		try
		{
			database_schema_manager.createContentMetadataTables();
		}
		catch (const std::exception& e)
		{
			std::cerr << "Failed to create content metadata tables. Application will be terminated. Exception: " << e.what() << std::endl;
			std::terminate();
		}
	}
	
	auto repository_factory = std::make_unique<ContentMetadataRepository::RepositoryFactory>(thread_pool_manager, date_time_parser);
	auto unit_of_work = std::make_shared<ContentMetadataRepository::UnitOfWork>(configuration, std::move(repository_factory), thread_pool_manager);

	try 
	{
		unit_of_work->initializeConnectionsStack(configuration->getThreadPoolLimit());
	}
	catch (const std::exception& e)
	{
		std::cerr << "Failed to create database connections. Application will be terminated. Exception: " << e.what() << std::endl;
		std::terminate();
	}

	//ContentMetadataCore<-Repository
	auto content_contract = std::make_shared<ContentMetadataRepository::ContentsDatabaseService>(unit_of_work);
	auto content_comments_contract = std::make_shared<ContentMetadataRepository::ContentCommentsDatabaseService>(unit_of_work);

	//ContentMetadataApi
	auto content_comment_mapper = std::make_shared<ContentMetadataApi::Mappers::ContentCommentDataMapper>();
	auto license_rules_mapper = std::make_shared<ContentMetadataApi::Mappers::LicenseRulesDataMapper>();
	auto content_mapper = std::make_shared<ContentMetadataApi::Mappers::ContentDataMapper>(content_comment_mapper, license_rules_mapper);
	
	auto guid_parser = std::make_shared<ContentMetadataApi::Parsers::GuidParser>();

	auto visitor_factory = std::make_shared<ContentMetadataApi::Visitors::VisitorFactory>(date_time_parser, guid_parser);

	auto http_command_foactories_factory = std::make_shared<ContentMetadataApi::Routing::HttpCommandFactoriesFactory>(
		content_contract,
		content_comments_contract,
		content_mapper,
		content_comment_mapper,
		guid_parser,
		visitor_factory);

	auto command_selector = std::make_unique<ContentMetadataApi::Routing::CommandSelector>(http_command_foactories_factory);

	auto endpoint_parser = std::make_shared<ContentMetadataApi::Parsers::EndpointParser>();
	auto http_data_mapper = std::make_unique<ContentMetadataApi::Mappers::HttpDataMapper>(endpoint_parser);

	auto server = new ContentMetadataApi::HttpServer(std::move(http_data_mapper), std::move(command_selector));

	server->startServer(configuration->getServerPort());

	std::cout << "\nServer running...\n" << std::endl;

	while (true) 
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	server->stopServer();
	delete server;
	thread_pool_manager->join();
	return 0;
}
