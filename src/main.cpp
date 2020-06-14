
#include <iostream>
#include <variant>
#include <vector>
#include <unordered_map>
#include <memory>

#include <stdexcept>

#define DEBUG_LOGS

#ifdef DEBUG_LOGS
	#define LOG(x) std::cout << x << std::endl
#else
	#define LOG(x)
#endif

/// <summary>
/// Enum identifier for every json type.
/// </summary>
enum class JSONTypeId
{
	JSON_NULL,
	JSON_OBJECT,
	JSON_VECTOR,
	JSON_BOOL,
	JSON_INT,
	JSON_DOUBLE,
	JSON_STRING
};

class JSONAttribute;
using JSONObject = std::unordered_map<std::string, JSONAttribute>;
using JSONArray = std::vector<JSONAttribute>;

// TODO: Very big Problem when const char* is passed as because client wants to create
// std::string. JSONAttribute::getIf() produces weird errors.
using JSONType = std::variant
<
	void*,
	JSONObject,
	JSONArray,
	bool,
	int,
	double,
	std::string
>;

using Access = std::variant<int, std::string>;

class JSONAttribute
{
public:
	JSONAttribute()
		: 
		type(JSONTypeId::JSON_NULL),
		data(nullptr)
	{

	}

	JSONAttribute(JSONTypeId type, JSONType data)
		:
		type(type),
		data(data)
	{

	}

	/*
	  Most errors in template stuff only appears when 
	  actually instantiating the template, because
	  before doing that there is no actual function
	  or whatever is used with templates.
	*/

	/*
		The Access std::variant<int, std::string> can/should only
		be used in a way such that only a std::vector<Access> of the 
		following 2 formats is passed to this function...

		{ "nested_object_0", "nested_object_1", ... , "nested_object_n",
		  "attribute_key" }

		OR { "nested_object_0", "nested_object_1", ... , "nested_object_n",
		  "attribute_key", i of [0, array.size() - 1] }

		The latter format is for getting a specific element out of 
		a JSONAttribute of type JSONArray.

	*/
	template <typename T>
	T const* getIf(std::vector<Access> const& keys) const
	{
		// Find out which keys format was passed.
		if (auto* isFirstFormat = std::get_if<std::string>(&keys.back()))
		{
			// Go to JSONObject that contains JSONAttribute that we seek.
			JSONObject const* tempObj = &std::get<JSONObject>(this->data);
			for (int i = 0; i < keys.size() - 1; ++i)
			{
				auto it = tempObj->find(std::get<std::string>(keys.at(i)));
				if (it == tempObj->end())
				{
					throw std::invalid_argument("Key doesnt exist.");
				}

				tempObj = &std::get<JSONObject>(it->second.data);
			}

			// Get the JSONAttribute that we seek.
			auto it = tempObj->find(std::get<std::string>(keys.back()));
			if (it == tempObj->end())
			{
				throw std::invalid_argument("Key doesnt exist.");
			}

			if (auto* value = std::get_if<T>(&it->second.data))
			{
				return value;
			}
			else
			{
				LOG("std::getIf<T>(): Couldn't get value. Template type T might be wrong.");
				return nullptr;
			}
		}

		else
		{
			// Go to JSONObject that contains JSONAttribute that we seek.
			JSONObject const* tempObj = &std::get<JSONObject>(this->data);
			for (int i = 0; i < keys.size() - 2; ++i)
			{
				auto it = tempObj->find(std::get<std::string>(keys.at(i)));
				if (it == tempObj->end())
				{
					throw std::invalid_argument("Key doesnt exist.");
				}

				tempObj = &std::get<JSONObject>(it->second.data);
			}

			// Get the JSONAttribute of type JSONArray that we seek.
			auto it = tempObj->find(std::get<std::string>(keys.at(keys.size() - 2)));
			if (it == tempObj->end())
			{
				throw std::invalid_argument("Key doesnt exist.");
			}

			// Get the element in that JSONArray and return it.
			JSONArray const* array = &std::get<JSONArray>(it->second.data);
			int arrayIndex = std::get<int>(keys.back());
			if (auto* element = std::get_if<T>(&(array->at(arrayIndex).data)))
			{
				return element;
			}
			else
			{
				LOG("std::getIf<T>(): Couldn't get value. Template type T might be wrong.");
				return nullptr;
			}
		}
	}

public:
	JSONTypeId type;
	JSONType data;
};

/*
		{
			"boolean": true,
			"nested":
			{
				"myString": "Hello, this is my string",
				"array": [true, 42]
			}
		}
*/

int main()
{
	JSONAttribute a
	{
		JSONTypeId::JSON_OBJECT,
		JSONObject
		{
			{
				{"boolean", JSONAttribute(JSONTypeId::JSON_BOOL, true)},
				{"nested", JSONAttribute(JSONTypeId::JSON_OBJECT, JSONObject
					{
						{
							{"myString", JSONAttribute(JSONTypeId::JSON_STRING, std::string("Hello, this is my string"))},
							{"array", JSONAttribute(JSONTypeId::JSON_VECTOR, JSONArray
							{
								JSONAttribute(JSONTypeId::JSON_BOOL, true),
								JSONAttribute(JSONTypeId::JSON_INT, 42),
								JSONAttribute(JSONTypeId::JSON_DOUBLE, 3.1418)
							})}
						}
					})}
			}
		}
	};

	bool boolean = *a.getIf<bool>({ "boolean" });
	std::string myString = *a.getIf<std::string>({ "nested", "myString" });

	double d = *a.getIf<double>({ "nested", "array", 2 });
	std::cout << d << std::endl;

	JSONArray const& array = *a.getIf<JSONArray>({ "nested", "array" });

	/*JSONAttribute integer = array.at(1);
	if (integer.type == JSONTypeId::JSON_INT)
	{
		std::cout << std::get<int>(integer.data) << std::endl;
	}*/

	std::cout << std::get<int>(array.at(1).data) << std::endl;

	std::cin.get();
}