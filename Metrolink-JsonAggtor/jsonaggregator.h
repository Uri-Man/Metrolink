#pragma once
#include "aggregation.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include <iostream>
#include <fstream>
#include <cstdio>

/*
Config grouby column name
*/
#define GROUPBY_COL "groupBy"

class JsonAggregator {
	/*
	Aggregates JSON logs to get sum of column
	*/

public:
	JsonAggregator(const string columnToSum) : columnToSum(columnToSum) {};

	/*
	Loads a JSON config file containing the Groupby details
	*/
	int loadConfig(const string configFile);

	/*
	Trys to parse a JSON file and updates aggregation
	*/
	int addJson(const string jsonFile);

	/*
	Writes aggregation results to file
	*/
	int writeResults(const string outFile);

private:
	const string columnToSum;

	// Trys to read and parse a JSON file based on groupby columns
	// @param path Path to file
	// @param json Json object to fill
	int readJson(const string path, rapidjson::Document& json);

	// Parses aggregation entry to JSON string
	string parseEntry(const AggKey& aggKey, float aggResult);

	// Groupby columns/keys
	vector<string> groupby;

	// Aggregation object
	SumAggregation agg;
	
};
