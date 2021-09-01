#pragma once
#include "aggregation.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/stringbuffer.h"

#include <iostream>
#include <cstdio>
#define RAPIDJSON_HAS_STDSTRING 1;
#define GROUPBY_COL "groupBy"
#define ON_COL "moneySpent"

class JsonAggregator {

public:
	JsonAggregator();
	int loadConfig(string configFile);
	int addJson(string jsonFile);
	int writeResults(string outFile);

private:
	int readJson(string path, rapidjson::Document& json, vector<string> requiredMembers);
	vector<string> groupby;
	string on;
	SumAggregation agg;
	
};
