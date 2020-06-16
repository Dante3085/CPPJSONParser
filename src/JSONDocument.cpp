
#include "pch.h"

#include "JSONDocument.h"

JSONDocument::JSONDocument(std::string const& filepath)
{
	rootObject = std::make_unique<JSONAttribute>(JSONTypeId::JSON_OBJECT, JSONObject());
	parseFile(filepath);
}

void JSONDocument::parseFile(std::string const& filepath)
{
	// Read file contents into string
	std::ifstream t(filepath);
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string fileContents = buffer.str();

	std::stack<JSONAttribute> objects

	for (char const& c : fileContents)
	{
		if ()
	}
}
