#include "jsonaggregator.h"

int JsonAggregator::loadConfig(const string configFile)
{

	rapidjson::Document config;
	if (readJson(configFile.c_str(), config)) {
		std::cout << "[!] Config load failed" << std::endl;
		return 1;
	}

	/*
	Validate config
	*/
	if (!config.HasMember(GROUPBY_COL)){
		std::cout << "[!] Config Does not have required key - " << GROUPBY_COL << std::endl;
		return 2;
	}	

	auto& groupbyCol = config[GROUPBY_COL];

	if (!groupbyCol.IsArray()) {
		std::cout << "[!] Config column must be an array - " << GROUPBY_COL << std::endl;
		return 3;
	}

	/*
	Store groupby columns
	*/
	for (const auto& col : groupbyCol.GetArray()) {
		if (!col.IsString()) {
			std::cout << "[!] All array elements should be string - " << GROUPBY_COL << std::endl;
			return 4;
		}
		this->groupby.push_back(col.GetString());
	}

	return 0;
}

int JsonAggregator::addJson(const string jsonFile)
{

	rapidjson::Document json;

	if (readJson(jsonFile.c_str(), json))
		return 1;
	
	/*
	Validate ON column and get value
	*/
	if (!json.HasMember(columnToSum.c_str())) {
		std::cout << "[*] JSON fail - '" << jsonFile << "' Does not have required key - " << columnToSum.c_str() << std::endl;
		return 2;
	}

	auto& onCol = json[columnToSum.c_str()];

	if (!onCol.IsFloat() && !onCol.IsInt()) {
		std::cout << "[*] JSON fail - '" << jsonFile << "' Property must be float - " << columnToSum.c_str() << std::endl;
		return 3;
	}


	/*
	Validate groupby columns and them to Agg Key
	*/
	AggKey aggKey;

	for (const auto& col : groupby) {
		auto& colValue = json[col.c_str()];
		if (!colValue.IsString()) {
			std::cout << "[*] JSON fail - '" << jsonFile << "' Property should be string - " << col << std::endl;
			return 4;
		}
		aggKey.push_back(colValue.GetString());
	}

	/*
	Update aggregation
	*/
	this->agg.update(aggKey, onCol.GetFloat());

	return 0;
}

int JsonAggregator::writeResults(const string outFile)
{
	/*
	File init
	*/
	std::ofstream filestream(outFile);
	filestream << "[";
	

	/*
	Parse and write each aggreagtion entry
	*/
	bool firstEntry = true;
	for (const auto& entry : this->agg.getEntries()) {

		if (!firstEntry)
			filestream << ",\n";
		else
			firstEntry = false;

		filestream << this->parseEntry(entry.first, entry.second);
	}

	/*
	File finish
	*/
	filestream << "]";
	filestream.close();

	return 0;
}

int JsonAggregator::readJson(const string path, rapidjson::Document& json)
{
	using namespace rapidjson;

	/*
	Handle to file
	*/
	FILE* fp = fopen(path.c_str(), "rb");

	if (NULL == fp) {
		std::cout << "[*] Could not open JSON file - " + path << std::endl;
		return 1;
	}

	/*
	Read and parse
	*/
	char readBuffer[1024];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	json.ParseStream(is);

	/*
	Validate
	*/
	if (json.HasParseError() || !json.IsObject()) {
		std::cout << "[*] Could not parse JSON file - " + path << std::endl;
		return 2;
	}
	
	return 0;
}

string JsonAggregator::parseEntry(const AggKey& aggKey, float aggResult)
{
	using namespace rapidjson;

	// Init JSON object and allocator
	Document d;
	d.SetObject();
	rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
	allocator.Clear();

	// Add result
	d.AddMember(
		Value(columnToSum.c_str(), allocator).Move(), // Result name
		aggResult, // Result value
		allocator);

	// Add key (groupby columns)
	for (int i = 0; i < aggKey.size(); i++)

		d.AddMember(
			Value(this->groupby[i].c_str(), allocator).Move(), // Column name
			Value(aggKey[i].c_str(), allocator).Move(),  // Column value
			allocator);

	// Convert JSON document to string
	rapidjson::StringBuffer buffer;
	buffer.Clear();
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	d.Accept(writer);
	
	return string(buffer.GetString(), buffer.GetSize());

}
