// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#include <vector>
#include <iostream>
#include <stack>
#include <queue>
#include <deque>
#include <string>
#include <cmath>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <getopt.h>
#include "zombie.h"

using namespace std;

struct Options
{
	unsigned int N;
	bool isVerbose;
	bool isStats;
	bool isMedian;
};

class CompareZombieETA 
{
public:
	bool operator()(zombie *z1, zombie *z2) const 
	{
		if (z1->ETA < z2->ETA)
		{
			return true;
		}
		else if (z1->ETA == z2->ETA)
		{
			if (z1->health < z2->health)
			{
				return true;
			}
			else if (z1->health == z2->health)
			{
				if (z1->name < z2->name)
				{
					return true;
				}
			}
		}
		return false;
	}
};

class CompareZombieAge
{
public:
	bool operator()(zombie *z1, zombie *z2) const
	{
		if (z1->age < z2->age)
		{
			return true;
		}
		else if (z1->age == z2->age)
		{
			if (z1->name < z2->name)
			{
				return true;
			}
		}
		return false;
	}
};


class game
{
private:
	vector<zombie> activeZombies;
	vector<zombie*> deadZombies;
	CompareZombieETA ETAless;
	CompareZombieAge ageLess;
	vector<int> ages;
	priority_queue<zombie*, vector<zombie*>, CompareZombieETA> sortedZombies;

public:
	string chadZombie;
	string lastJedi;
	int round;
	int arrows;

	// header variables
	int quiverCap;
	int randSeed;
	int maxRandDist;
	int maxRanDSpeed;
	int maxRandHP;

	unsigned int N;

	// round variables
	bool isDead = false;
	bool isGameWon;
	bool isVerbose;
	bool isStats;
	bool isMedian;
	bool didZombieDie = false;

	game (Options mode_in)
		: N(mode_in.N), isVerbose(mode_in.isVerbose), isStats(mode_in.isStats), isMedian(mode_in.isMedian)
	{}

	void zombies_attack()
	{
		if (!activeZombies.empty())
		{
			for (size_t i = 0; i < activeZombies.size(); i++)
			{
				activeZombies[i].move(isDead, isVerbose, chadZombie);
			}
			if (isDead)
			{
				return;
			}
		}
	}

	void human_attack()
	{
		while (arrows != 0 || !sortedZombies.empty())
		{
			while (sortedZombies.top()->health != 0 || arrows != 0)
			{
				sortedZombies.top()->health--;
				arrows--;
			}
			if (arrows != 0)
			{
				if (sortedZombies.top()->health == 0)
				{
					if (isVerbose)
					{
						cout << "Destroyed: " << sortedZombies.top()->name << " (distance: " << sortedZombies.top()->distance << ", speed: " << sortedZombies.top()->speed << ", health: " << sortedZombies.top()->health << ")" << endl;
					}
					didZombieDie = true;
					sortedZombies.top()->isActive = false;
					ages.push_back(sortedZombies.top()->age);
					deadZombies.push_back(sortedZombies.top());
					sortedZombies.pop();
				}
				return;
			}
			if (sortedZombies.top()->health == 0)
			{
				if (isVerbose)
				{
					cout << "Destroyed: " << sortedZombies.top()->name << " (distance: " << sortedZombies.top()->distance << ", speed: " << sortedZombies.top()->speed << ", health: " << sortedZombies.top()->health << ")" << endl;
				}
				didZombieDie = true;
				sortedZombies.top()->isActive = false;
				ages.push_back(sortedZombies.top()->age);
				deadZombies.push_back(sortedZombies.top());
				sortedZombies.pop();
			}
			if (sortedZombies.empty())
			{
				isGameWon = true;
				lastJedi = sortedZombies.top()->name;
				return;
			}
		}
	}

	int get_median()
	{
		sort(ages.begin(), ages.end());
		if (ages.size() % 2 != 0)
		{
			return ages[ages.size() / 2];
		}
		return (ages[(ages.size() - 1) / 2] + ages[ages.size() / 2]) / 2;
	}

	void read_header()
	{
		string key;
		getline(cin, key);
		// read in header
		cin >> key >> quiverCap;
		cin >> key >> randSeed;
		cin >> key >> maxRandDist;
		cin >> key >> maxRanDSpeed;
		cin >> key >> maxRandHP;
	}

	void read_round(int count)
	{
		string indicator;
		getline(cin, indicator);
		assert(indicator == "---");
		string key;

		int randZombies;
		int namedZombies;
		int roundNum;

		cin >> key >> roundNum;
		cin >> key >> randZombies;
		cin >> key >> namedZombies;
		if (count == roundNum)
		{
			string name, key1, key2;
			int dist, speed, hp;

			for (int i = 0; i < randZombies; i++)
			{
				name = P2random::getNextZombieName();
				dist = P2random::getNextZombieDistance();
				speed = P2random::getNextZombieSpeed();
				hp = P2random::getNextZombieHealth();
				zombie temp(name, dist, speed, hp, roundNum);
				if (isVerbose)
				{
					cout << "Created: " << name << " (distance: " << dist << ", speed: " << speed << ", health: " << hp << ")" << endl;
				}
				activeZombies.push_back(temp);
				sortedZombies.push(&temp);
			}

			for (int i = 0; i < namedZombies; i++)
			{
				cin >> name >> key >> dist >> key1 >> speed >> key2 >> hp;
				zombie temp(name, dist, speed, hp, roundNum);
				if (isVerbose)
				{
					cout << "Created: " << name << " (distance: " << dist << ", speed: " << speed << ", health: " << hp << ")" << endl;
				}
				activeZombies.push_back(temp);
				sortedZombies.push(&temp);
			}
		}
	}

	void print_stats()
	{
		cout << "Zombies still active: " << activeZombies.size() - deadZombies.size() << endl;
		size_t min = deadZombies.size();
		if (N < deadZombies.size())
		{
			min = N;
		}
		cout << "First zombies killed:" << endl;
		for (size_t i = 0; i != min; i++)
		{
			cout << deadZombies[i]->name << " " << i + 1 << endl;
		}

		if (min == deadZombies.size())
		{
			min--;
		}

		cout << "Last zombies killed:" << endl;
		for (size_t i = min; i > 0; i--)
		{
			cout << deadZombies[i]->name << " " << min - i + 1 << endl;
		}
		cout << deadZombies[0]->name << " " << min + 1 << endl;

		sort(deadZombies.begin(), deadZombies.end(), ageLess);

		cout << "Most active zombies:" << endl;
		for (size_t i = deadZombies.size() - 1; i > 0; i--)
		{
			cout << deadZombies[i]->name << " " << deadZombies[i]->age << endl;
		}
		cout << deadZombies[0]->name << " " << min + 1 << endl;

		cout << "Least active zombies:" << endl;
		for (size_t i = 0; i < deadZombies.size(); i++)
		{
			if (deadZombies[i]->age > 0)
			{
				cout << deadZombies[i]->name << " " << deadZombies[i]->age << endl;
			}
		}
	}

};

