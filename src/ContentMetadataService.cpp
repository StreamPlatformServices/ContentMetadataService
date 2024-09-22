#include "ContentMetadataService.hpp"
#include "../ContentMetadataCore/ContentsService.hpp"
#include "../ContentMetadataCore/ContentCommentsService.hpp"
#include "../ContentMetadataApi/Mappers/EntityMappers/ContentCommentDataMapper.hpp"
#include "../ContentMetadataApi/Mappers/EntityMappers/LicenseRulesDataMapper.hpp"
#include "../ContentMetadataApi/Mappers/EntityMappers/ContentDataMapper.hpp"
#include "../ContentMetadataApi/Routing/Factories/HttpCommandFactoriesFactory.hpp"
#include "../ContentMetadataApi/Routing/CommandSelector.hpp"
#include "../ContentMetadataApi/Parsers/EndpointParser.hpp"
#include "../ContentMetadataApi/Parsers/DateTimeParser.hpp"
#include "../ContentMetadataApi/Parsers/GuidParser.hpp"
#include "../ContentMetadataApi/Mappers/HttpDataMapper.hpp"
#include "../ContentMetadataApi/Visitors/Factories/VisitorFactory.hpp"
#include "../ContentMetadataApi/HttpServer.hpp"

//TODO: initialize async spdlog and pass throught dependency injection
//logging in validator

int main()
{
	//ContentMetadataCore
	auto content_contract = std::make_shared<ContentMetadataCore::ContentsService>();
	auto content_comments_contract = std::make_shared<ContentMetadataCore::ContentCommentsService>();


	//ContentMetadataApi
	auto content_comment_mapper = std::make_shared<ContentMetadataApi::Mappers::ContentCommentDataMapper>();
	auto license_rules_mapper = std::make_shared<ContentMetadataApi::Mappers::LicenseRulesDataMapper>();
	auto content_mapper = std::make_shared<ContentMetadataApi::Mappers::ContentDataMapper>(content_comment_mapper, license_rules_mapper);
	
	auto guid_parser = std::make_shared<ContentMetadataApi::Parsers::GuidParser>();

	auto date_time_parser = std::make_shared<ContentMetadataApi::Parsers::DateTimeParser>();
	auto visitor_factory = std::make_shared<ContentMetadataApi::Visitors::VisitorFactory>(date_time_parser, guid_parser);

	auto http_command_foactories_factory = std::make_shared<ContentMetadataApi::Routing::HttpCommandFactoriesFactory>(
		content_contract,
		content_comments_contract,
		content_mapper,
		content_comment_mapper,
		guid_parser,
		visitor_factory);

	auto command_selector = std::make_unique<ContentMetadataApi::Routing::CommandSelector>(http_command_foactories_factory);

	auto endpoint_parser = std::make_unique<ContentMetadataApi::Parsers::EndpointParser>();
	auto http_data_mapper = std::make_unique<ContentMetadataApi::Mappers::HttpDataMapper>(std::move(endpoint_parser));

	auto server = new ContentMetadataApi::HttpServer(std::move(http_data_mapper), std::move(command_selector));

	server->startServer(5050);
	std::cout << "Hello CMake." << std::endl;

	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	server->stopServer();
	std::cout << "Hello Server." << std::endl;

	delete server;
	return 0;
}
