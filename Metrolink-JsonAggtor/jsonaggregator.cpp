#include "jsonaggregator.h"

int JsonAggregator::loadConfig(string configFile)
{

	rapidjson::Document config;
	if (!readJson(configFile.c_str(), config)) {
		std::cout << "[!] Config load failed" << std::endl;
		return 1;
	}

	if (!config.HasMember(GROUPBY_COL)){
		std::cout << "[!] Config Does not have required key - " << GROUPBY_COL << std::endl;
		return 2;
	}	

	auto& groupbyCol = config[GROUPBY_COL];

	if (!groupbyCol.IsArray()) {
		std::cout << "[!] Config column must be an array - " << GROUPBY_COL << std::endl;
		return 3;
	}

	for (const auto& col : groupbyCol.GetArray()) {
		if (!col.IsString()) {
			std::cout << "[!] All array elements should be string - " << GROUPBY_COL << std::endl;
			return 4;
		}
		this->groupby.push_back(col.GetString());
	}

	return 0;
}

int JsonAggregator::addJson(string jsonFile)
{

	rapidjson::Document json;
	if (!readJson(jsonFile.c_str(), json))
		return 1;

	if (!json.HasMember(ON_COL)) {
		std::cout << "[*] Does not have required key - " << ON_COL << std::endl;
		return 2;
	}

	auto& onCol = json[ON_COL];

	if (!onCol.IsFloat() && !onCol.IsInt()) {
		std::cout << "[!] Config column must be an string - " << ON_COL << std::endl;
		return 3;
	}

	AggKey aggKey;

	for (const auto& col : groupby) {
		auto& colValue = json[col.c_str()];
		if (!colValue.IsString()) {
			std::cout << "[!] All array elements should be string - " << GROUPBY_COL << std::endl;
			return 4;
		}
		aggKey.push_back(colValue.GetString());
	}

	this->agg.add(aggKey, onCol.GetFloat());

	return 0;
}

int JsonAggregator::writeResults(string outFile)
{
	using namespace rapidjson;

	Document d;
	d.SetObject();

	rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

	size_t sz = allocator.Size();

	d.AddMember("version", "a", allocator);
	d.AddMember("testId", 2, allocator);
	d.AddMember("group", 3, allocator);
	d.AddMember("order", 4, allocator);
	

	Value tests(kArrayType);
	Value obj(kObjectType);
	Value val(kObjectType);

	obj.AddMember("id", 1, allocator);

	string description = "a description";
	val.SetString(description.c_str(), static_cast<SizeType>(description.length()), allocator);
	obj.AddMember("description", val, allocator);

	string help = "some help";
	val.SetString(help.c_str(), static_cast<SizeType>(help.length()), allocator);
	obj.AddMember("help", val, allocator);

	string workgroup = "a workgroup";
	val.SetString(workgroup.c_str(), static_cast<SizeType>(workgroup.length()), allocator);
	obj.AddMember("workgroup", val, allocator);

	val.SetBool(true);
	obj.AddMember("online", val, allocator);

	tests.PushBack(obj, allocator);
	d.AddMember("tests", tests, allocator);

	// Convert JSON document to string
	rapidjson::StringBuffer strbuf;
}

int JsonAggregator::readJson(string path, rapidjson::Document & json, vector<string> requiredMembers)
{
	using namespace rapidjson;

	FILE* fp = fopen(path.c_str(), "rb");

	if (NULL == fp) {
		std::cout << "[*] Could not open JSON file - " + path << std::endl;
		return 1;
	}

	char readBuffer[1024];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	json.ParseStream(is);

	if (json.HasParseError()) {
		std::cout << "[*] Could not parse JSON file - " + path << std::endl;
		return 2;
	}

	return 0;
}
