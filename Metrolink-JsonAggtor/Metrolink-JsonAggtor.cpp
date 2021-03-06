
#include "jsonaggregator.h"
#include <filesystem>

#define COLUMN_TO_SUM "moneySpent"

int main(int argc, char**argv)
{
	// Help
	if (argc < 3)
		std::cout << "Usage: MetroLink-JsonAggtor.exe <Input Folder> <Config file>";

	else {

		// Init json agg object
		auto ja=JsonAggregator(COLUMN_TO_SUM);

		// Load config from second argument
		if (ja.loadConfig(argv[2])) {
			std::cout << "Exiting..";
			exit(1);
		}

		// Enum all files in folder passed to first argument,
		// and try to them to aggregation
		for (const auto & entry : std::experimental::filesystem::directory_iterator(argv[1]))
			ja.addJson(entry.path().string());
		
		// Write result to given folder
		ja.writeResults(argv[1] + string("\\results.json"));

	}
    return 0;
}

