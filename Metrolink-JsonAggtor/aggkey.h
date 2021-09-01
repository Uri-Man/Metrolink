#pragma once
#include <vector>
#include <string>

using std::vector;
using std::string;

/*
Represents a key composed of multiple strings
*/
typedef vector<string> AggKey;

/*
Add a hash function for AggKey to std namespace
so it can be used with map objects
*/
namespace std {

	template <>
	struct hash<AggKey>
	{
		std::size_t operator()(const AggKey& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			std::size_t res = 0;

			// XOR hashes of all strings and salt with index hash
			int i = 0;
			for (auto s : k) {
				res ^= hash<string>()(s);
				res ^= hash<int>()(i);
				i++;
			}

			return res;
		}
	};
}