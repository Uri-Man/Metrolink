#pragma once
#include <unordered_map>
#include "aggkey.h"

using std::unordered_map;


class ArithmeticAggregation {
public:
	ArithmeticAggregation(float defaltValue) : defaltValue(defaltValue) {};

	/*
	Updates aggregtion of key with given value
	*/
	void update(AggKey key, float value);
	const unordered_map<AggKey, float>& getEntries();

protected:
	/*
	Aggregation function 
	*/
	virtual float aggFunc(float prev, float value) = 0;

private:
	
	// Value to start aggregation
	const float defaltValue;

	// Aggregation entries
	unordered_map<AggKey, float> entries;
};

class SumAggregation : public ArithmeticAggregation {
	/*
	Implements sum aggregation
	*/
public:

	/*
	Init value of sum is 0
	*/
	SumAggregation() : ArithmeticAggregation(0) {};
private:

	/*
	Implemented as addition
	*/
	float aggFunc(float prev, float value);
};

