
#include "pch.h"

#include "JSONDocument.h"

JSONDocument::JSONDocument(std::string const& filepath)
{
	parseFile(filepath);
}

void JSONDocument::parseFile(std::string const& filepath)
{
	// Read file contents into string
	std::ifstream t(filepath);
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string fileContents = buffer.str();

	std::cout << fileContents << std::endl;
}
