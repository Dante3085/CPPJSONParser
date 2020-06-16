#pragma once

#include "JSONData.h"

class JSONDocument
{
public:
	JSONDocument(std::string const& filepath);

private:
	void parseFile(std::string const& filepath);

private:
	std::unique_ptr<JSONAttribute> rootObject;
};

