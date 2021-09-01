
#include "jsonaggregator.h"
#include <filesystem>
#include <chrono>


int main(int argc, char**argv)
{
	/*
	string aa;
	aa.append("asdf");
	
	// 1. Parse a JSON string into DOM.

	const char* json = "{\"project\":\"rapidjson\",\"stars\":\"zzz\",\"asdf\":\"qwer\"}";
	Document d;
	vector<string> v = { "project", "stars", "asdf" };
	d.Parse(json);
	auto& x = d['adsf'];
	try
	v.push_back(x.GetString());*/

	if (argc < 3)
		std::cout << "Usage: MetroLink-JsonAggtor.exe <Input Folder> <Config file>";

	else {

		auto ja=JsonAggregator();

		if (!ja.loadConfig(argv[2])) {
			exit(1);
		}

		for (const auto & entry : std::experimental::filesystem::directory_iterator(argv[1]))
			ja.addJson(entry.path().string());
		
		ja.writeResults(argv[1] + string("\\results.json"));

	}
    return 0;
}

