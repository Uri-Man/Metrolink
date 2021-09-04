#include "aggregation.h"

void ArithmeticAggregation::update(AggKey key, float value)
{
	// Try to find relevant entry
	auto prev = entries.find(key);

	// Update entry
	if (prev != entries.end())
		prev->second = aggFunc(prev->second, value);

	// Init entry
	else
		entries[key] = aggFunc(defaltValue, value);
}

const unordered_map<AggKey, float>& ArithmeticAggregation::getEntries()
{
	return entries;
}

float SumAggregation::aggFunc(float prev, float value)
{
	/*
	Add new value to previous sum
	*/
	return prev + value;
}
