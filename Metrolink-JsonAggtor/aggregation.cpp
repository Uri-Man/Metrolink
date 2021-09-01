#include "aggregation.h"

void ArithmeticAggregation::add(AggKey key, float value)
{
	auto prev = agg.find(key);

	// Update aggregation
	if (prev != agg.end())
		prev->second = aggFunc(prev->second, value);

	// Init aggregation
	else
		agg[key] = aggFunc(defaltValue, value);
}

float SumAggregation::aggFunc(float prev, float value)
{
	/*
	Sum new value with previous sum
	*/
	return prev + value;
}
