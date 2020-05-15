// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <string>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <getopt.h>
#include "game.h"
#include "zombie.h"

using namespace std;

struct Options
{
	bool isVerbose;
	bool isStats;
	bool isMedian;
};


Options getMode(int argc, char * argv[]) {
	//bool modeSpecified = false;
	Options gameOptions;
	gameOptions.isVerbose = false;
	gameOptions.isStats = false;
	gameOptions.isMedian = false;
	string str;
	int count = 0;
	// These are used with getopt_long()
	opterr = true; // Give us help with errors
	int choice;
	int option_index = 0;
	option long_options[] = {
		{ "help", no_argument, nullptr, 'h' },
		{ "verbose", no_argument, nullptr, 'v' },
		{ "stats", required_argument, nullptr, 's' },
		{ "median", no_argument, nullptr, 'm' },
		{ nullptr, 0,                 nullptr, '\0' }
	};
	while ((choice = getopt_long(argc, argv, "hvs:m", long_options, &option_index)) != -1) {

		switch (choice) {

		case 'h':
		{
			cout << "Allowed options: -c, -f, -o, -v, -s, -p" << "\n";
			exit(1);
			break;
		}
		case 'v':
		{
			gameOptions.isVerbose = true;
			break;
		}
		case 's':
		{
			gameOptions.isStats = true;
			break;
		}
		case 'm':
		{
			gameOptions.isMedian = true;
			break;
		}
		default:
			cerr << "Error: invalid option" << endl;
			exit(1);
			// switch
		} // switch
	} // while

	return gameOptions;
} // getMode()


int main(int argc, char * argv[])
{
	std::ios_base::sync_with_stdio(false);

	Options mode = getMode(argc, argv);

	// main game loop
	while (true)
	{

	}
}



