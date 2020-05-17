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
#include "P2random.h"

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
		if (z1->ETA > z2->ETA)
		{
			return true;
		}
		else if (z1->ETA == z2->ETA)
		{
			if (z1->health > z2->health)
			{
				return true;
			}
			else if (z1->health == z2->health)
			{
				if (z1->name > z2->name)
				{
					return true;
				}
			}
		}
		return false;
	}
};

class CompareZombieAgeLess
{
public:
	bool operator()(zombie &z1, zombie &z2) const
	{
		if (z1.age < z2.age)
		{
			return true;
		}
		else if (z1.age == z2.age)
		{
			if (z1.name < z2.name)
			{
				return true;
			}
		}
		return false;
	}
};

class CompareZombieAgeMore
{
public:
	bool operator()(zombie &z1, zombie &z2) const
	{
		if (z1.age > z2.age)
		{
			return true;
		}
		else if (z1.age == z2.age)
		{
			if (z1.name < z2.name)
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
	

public:
	deque<zombie> activeZombies;
	deque<zombie*> deadZombies;
	CompareZombieETA ETAless;
	CompareZombieAgeLess ageLess;
	CompareZombieAgeMore ageMore;
	vector<int> ages;
	priority_queue<zombie*, deque<zombie*>, CompareZombieETA> sortedZombies;

	string chadZombie;
	string lastJedi;
	int round;
	int arrows;
	int totalZombies = 0;

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
		//cout << "entered human attack" << "\n";
		//cout << "sortedZombie size: " << sortedZombies.size() << "\n";
		while (arrows != 0 || !sortedZombies.empty())
		{
			while (sortedZombies.top()->health != 0 || arrows != 0)
			{
				sortedZombies.top()->health--;
				arrows--;
				if (sortedZombies.top()->health == 0)
				{
					break;
				}
				if (arrows == 0)
				{
					break;
				}
			}
			if (arrows == 0)
			{
				if (sortedZombies.top()->health == 0)
				{
					if (isVerbose)
					{
						cout << "Destroyed: " << sortedZombies.top()->name << " (distance: " << sortedZombies.top()->distance << ", speed: " << sortedZombies.top()->speed << ", health: " << sortedZombies.top()->health << ")" << "\n";
					}
					didZombieDie = true;
					sortedZombies.top()->isActive = false;
					sortedZombies.top()->age++;
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
					cout << "Destroyed: " << sortedZombies.top()->name << " (distance: " << sortedZombies.top()->distance << ", speed: " << sortedZombies.top()->speed << ", health: " << sortedZombies.top()->health << ")" << "\n";
				}
				didZombieDie = true;
				sortedZombies.top()->isActive = false;
				sortedZombies.top()->age++;
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
		//cout << "exited human attack" << "\n";
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
		//cout << key << "\n";
		// read in header
		cin >> key >> quiverCap;
		//cout << quiverCap << "\n";
		cin >> key >> randSeed;
		//cout << randSeed << "\n";
		cin >> key >> maxRandDist;
		//cout << maxRandDist << "\n";
		cin >> key >> maxRanDSpeed;
		//cout << maxRanDSpeed << "\n";
		cin >> key >> maxRandHP;
		//cout << maxRandHP << "\n";
	}

	void read_round(int count)
	{
		//cout << "entered read_round" << "\n";
		string indicator;
		while (cin >> indicator)
		{
			//cout << indicator << "\n";
			if (indicator == "---")
			{
				break;
			}
		}
		string key;

		int randZombies;
		int namedZombies;
		int roundNum;

		cin >> key >> roundNum;
		cin >> key >> randZombies;
		cin >> key >> namedZombies;
		//cout << "round information: " << roundNum << randZombies << namedZombies << "\n";
		if (count == roundNum)
		{
			//cout << "passed if statement " << "\n";
			string name, key1, key2;
			unsigned int dist, speed, hp;

			for (int i = 0; i < randZombies; i++)
			{
				name = P2random::getNextZombieName();
				dist = P2random::getNextZombieDistance();
				speed = P2random::getNextZombieSpeed();
				hp = P2random::getNextZombieHealth();
				zombie temp(name, dist, speed, hp, roundNum);
				if (isVerbose)
				{
					cout << "Created: " << name << " (distance: " << dist << ", speed: " << speed << ", health: " << hp << ")" << "\n";
				}
				activeZombies.push_back(temp);
				sortedZombies.push(&activeZombies[totalZombies]);
				totalZombies++;
				//cout << "container sizes: " << activeZombies.size() << " " << sortedZombies.size() << "\n";
			}

			for (int i = 0; i < namedZombies; i++)
			{
				cin >> name >> key >> dist >> key1 >> speed >> key2 >> hp;
				zombie temp(name, dist, speed, hp, roundNum);
				if (isVerbose)
				{
					cout << "Created: " << name << " (distance: " << dist << ", speed: " << speed << ", health: " << hp << ")" << "\n";
				}
				activeZombies.push_back(temp);
				sortedZombies.push(&activeZombies[totalZombies]);
				totalZombies++;
				//cout << activeZombies.size() << " " << sortedZombies.size() << "\n";
			}
		}
		//cout << "finished read_round" << "\n";
	}

	void update_age()
	{
		for (size_t i = 0; i < activeZombies.size(); i++)
		{
			if (activeZombies[i].isActive || activeZombies[i].moved)
			{
				activeZombies[i].age++;
			}
		}
	}

	void print_test()
	{
		for (size_t i = 0; i < activeZombies.size(); i++)
		{
			cout << activeZombies[i].name << " " << sortedZombies.top()->name << "\n";
			sortedZombies.pop();
		}
	}

	void print_stats()
	{
		cout << "Zombies still active: " << activeZombies.size() - deadZombies.size() << "\n";
		
		cout << "First zombies killed:" << "\n";

		size_t min = deadZombies.size();
		if (N < deadZombies.size())
		{
			min = N;
		}

		for (size_t i = 0; i < min; i++)
		{
			cout << deadZombies[i]->name << " " << i + 1 << "\n";
		}

		if (min == deadZombies.size())
		{
			min--;
		}

		cout << "Last zombies killed:" << "\n";
		for (size_t i = min; i > 0; i--)
		{
			cout << deadZombies[i]->name << " " << i + 1 << "\n";
		}
		cout << deadZombies[0]->name << " " << 1 << "\n";

		cout << "Most active zombies:" << "\n";

		sort(activeZombies.begin(), activeZombies.end(), ageMore);

		size_t count1 = 1;

		for (size_t i = 0; i < activeZombies.size() && N != count1 - 1; i++)
		{
			if (activeZombies[i].age > 0)
			{
				cout << activeZombies[i].name << " " << activeZombies[i].age << "\n";
				count1++;
			}
		}
		
		cout << "Least active zombies:" << "\n";

		sort(activeZombies.begin(), activeZombies.end(), ageLess);

		count1 = 1;

		for (size_t i = 0; i < activeZombies.size() && N != count1 - 1; i++)
		{
			if (activeZombies[i].age > 0)
			{
				cout << activeZombies[i].name << " " << activeZombies[i].age << "\n";
				count1++;
			}
		}
		
	}

};

