#pragma once
#include <unordered_map>
#include "aggkey.h"

using std::unordered_map;


class ArithmeticAggregation {
public:
	ArithmeticAggregation(float defaltValue) : defaltValue(defaltValue) {};
	void add(AggKey key, float value);

protected:
	/*
	Aggregation function 
	*/
	virtual float aggFunc(float prev, float value) = 0;

private:
	const float defaltValue;
	unordered_map<AggKey, float> agg;
};

class SumAggregation : public ArithmeticAggregation {
public:
	SumAggregation() : ArithmeticAggregation(0) {};
private:
	float aggFunc(float prev, float value);
};

