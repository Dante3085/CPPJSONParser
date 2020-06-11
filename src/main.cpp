
#include <iostream>
#include <variant>
#include <vector>
#include <unordered_map>

using JSONType = std::variant
<
	bool,
	int,
	float,
	double,
	std::string,

	std::vector<bool>,
	std::vector<int>,
	std::vector<float>,
	std::vector<double>,
	std::vector<std::string>
>;

struct JSONObject
{
	std::unordered_map<std::string, std::variant<JSONType, JSONObject>> attributes;

	template <class T>
	T* getIf(std::vector<std::string> const& keys)
	{
		JSONObject* temp = this;

		// Go to JSONObject where last keys attribute resides.
		for (int i = 0; i < (keys.size() - 1); ++i)
		{
			temp = &std::get<JSONObject>(temp->attributes[keys[i]]);
		}

		// Find the attribute that we actually want to return,
		// which is the attribute that is pointed to by
		// the last given key.
		JSONType& variant = std::get<JSONType>(temp->attributes[keys[keys.size() - 1]]);

		// Check if the given template type T is the same type
		// that the attribute that we want to return has.
		if (auto* value = std::get_if<T>(&variant))
		{
			return value;
		}
		else
		{
			return nullptr;
		}
	}
};

int main()
{
	JSONObject o
	{ // Initialization brackets
		{ // unordered_map brackets
			{ "boolean", std::variant<JSONType, JSONObject>(true) }, // map entry brackets
			{ "nested_object", std::variant<JSONType, JSONObject>(JSONObject
				{
					{
						{ "float", std::variant<JSONType, JSONObject>(3.14123f)},
						{ "nested_object_2", std::variant<JSONType, JSONObject>(JSONObject
							{
								{
									{ "string", std::variant<JSONType, JSONObject>(std::string("Hello World"))}
								}
							}
						)},
						{ "numbers", std::variant<JSONType, JSONObject>(std::vector<int>{1, 2, 3, 4, 5}) },
						{ "num_1", std::variant<JSONType, JSONObject>(1) },
						{ "num_2", std::variant<JSONType, JSONObject>(2) },
						{ "num_3", std::variant<JSONType, JSONObject>(3) },
						{ "num_4", std::variant<JSONType, JSONObject>(4) },
						{ "num_5", std::variant<JSONType, JSONObject>(9999) },
					}
				} 
			)}
		}
	};

	bool boolean = *o.getIf<bool>({ "boolean" });
	float flo = *o.getIf<float>({ "nested_object", "float" });
	std::string string = *o.getIf<std::string>({ "nested_object", "nested_object_2", "string" });
	std::vector<int> numbers = *o.getIf<std::vector<int>>({ "nested_object", "numbers" });
	int num_1 = *o.getIf<int>({ "nested_object", "num_1" });
	int num_2 = *o.getIf<int>({ "nested_object", "num_2" });
	int num_3 = *o.getIf<int>({ "nested_object", "num_3" });
	int num_4 = *o.getIf<int>({ "nested_object", "num_4" });
	int num_5 = *o.getIf<int>({ "nested_object", "num_5" });

	std::cout << std::boolalpha << boolean << std::endl;
	std::cout << flo << std::endl;
	std::cout << string << std::endl;
	for (int i : numbers)
		std::cout << i << " ";
	std::cout << std::endl;
	std::cout << num_1 << std::endl;
	std::cout << num_2 << std::endl;
	std::cout << num_3 << std::endl;
	std::cout << num_4 << std::endl;
	std::cout << num_5 << std::endl;

	// Das obige JSONObject entspricht dem folgenden json file...
	/*
		{
			"boolean": true,
			"nested_object":
			{
				"float": 3.14123f,
				"nested_object_2":
				{
					"string": "Hello World"
				},
				"numbers": [1, 2, 3, 4, 5]
			}
		}
	*/

	std::cin.get();
}