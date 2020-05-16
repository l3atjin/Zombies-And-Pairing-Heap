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
#include "P2random.h"
#include "game.h"

using namespace std;




Options getMode(int argc, char * argv[]) {
	//bool modeSpecified = false;
	Options gameOptions;
	gameOptions.N = 0;
	gameOptions.isVerbose = false;
	gameOptions.isStats = false;
	gameOptions.isMedian = false;
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
			gameOptions.N = atoi(optarg);
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

	game pandemic(mode);

	pandemic.read_header();
	P2random::initialize(pandemic.randSeed, pandemic.maxRandDist, pandemic.maxRanDSpeed, pandemic.maxRandHP);
	
	int count = 1;
	// main game loop
	while (!pandemic.isDead || pandemic.isGameWon)
	{
		if (pandemic.isVerbose)
		{
			cout << "Round: " << count << endl;
		}
		pandemic.arrows = pandemic.quiverCap;

		// Update active zombies
		pandemic.zombies_attack();
		if (pandemic.isDead)
		{
			break;
		}
		// Read new round and create new zombies
		pandemic.read_round(count);

		cout << pandemic.activeZombies.size() << " " << pandemic.sortedZombies.size() << endl;
		// Shoot down dem zombies
		pandemic.human_attack();
		if (pandemic.isMedian && pandemic.didZombieDie)
		{
			cout << "At the end of round " << count << ", the median zombie lifetime is " << pandemic.get_median() << endl;
		}
		if (!pandemic.isGameWon && !pandemic.isDead)
		{
			count++;
		}

		cout << "finished loop" << endl;
	}
	if (pandemic.isGameWon)
	{
		cout << "VICTORY IN ROUND " << count << "! " << pandemic.lastJedi << " was the last zombie." << endl;
	}
	else if (pandemic.isDead)
	{
		cout << "DEFEAT IN ROUND " << count << pandemic.chadZombie << " ate your brains!" << endl;
	}
	if (pandemic.isStats)
	{
		pandemic.print_stats();
	}
}



