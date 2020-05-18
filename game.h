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
priority_queue<zombie*, deque<zombie*>, CompareZombieETA> sortedZombies;
priority_queue<zombie*, deque<zombie*>, CompareZombieETA> mostActiveZombies;
priority_queue<zombie*, deque<zombie*>, CompareZombieETA> leastActiveZombies;
priority_queue<int, deque<int>, greater<int>> upper;
priority_queue<int, deque<int>, less<int>> lower;

string chadZombie;
string lastJedi;
string indicator;
int lastRound;
int arrows;
int totalZombies = 0;

// header variables
int quiverCap;
int randSeed;
int maxRandDist;
int maxRanDSpeed;
int maxRandHP;
int roundNum;

unsigned int N;

// round variables
bool isDead = false;
bool isGameWon = false;
bool isVerbose;
bool isStats;
bool isMedian;
bool didZombieDie = false;
bool skipped = false;
bool nextRound = true;

game (Options mode_in)
	: N(mode_in.N), isVerbose(mode_in.isVerbose), isStats(mode_in.isStats), isMedian(mode_in.isMedian)
{}

void zombies_attack(int count_in)
{
	if (!activeZombies.empty())
	{
		for (size_t i = 0; i < activeZombies.size(); i++)
		{
			activeZombies[i].move(isDead, isVerbose, chadZombie);
		}
		if (isDead)
		{
			lastRound = count_in;
			return;
		}
	}
}

void median_helper(int in)
{
	if (!didZombieDie)
	{
		upper.push(in);
	}
	else if (in >= upper.top())
	{
		upper.push(in);
	}
	else
	{
		lower.push(in);
	}
	// balance
	if (upper.size() - lower.size() == 2) 
	{ 
		lower.push(upper.top());
		upper.pop();
	}
	else if (lower.size() - upper.size() == 2) 
	{
		upper.push(lower.top());
		lower.pop();
	}
}

void human_attack(int count_in)
{
	//cout << "entered human attack" << "\n";
	//cout << "sortedZombie size: " << sortedZombies.size() << "\n";
	if (!isDead && !sortedZombies.empty() && !isGameWon)
	{
		while (arrows != 0 && !sortedZombies.empty())
		{
			if (arrows >= sortedZombies.top()->health)
			{
				arrows = arrows - sortedZombies.top()->health;
				sortedZombies.top()->health = 0;
				if (isVerbose)
				{
					cout << "Destroyed: " << sortedZombies.top()->name << " (distance: " << sortedZombies.top()->distance << ", speed: " << sortedZombies.top()->speed << ", health: " << sortedZombies.top()->health << ")" << "\n";
				}
				sortedZombies.top()->isActive = false;
				sortedZombies.top()->roundDied = count_in;
				sortedZombies.top()->age = count_in - sortedZombies.top()->roundBorn + 1;
				//ages.push_back(sortedZombies.top()->age);
				median_helper(sortedZombies.top()->age);
				deadZombies.push_back(sortedZombies.top());
				didZombieDie = true;
				if (sortedZombies.size() == 1)
				{
					lastJedi = sortedZombies.top()->name;
				}
				sortedZombies.pop();
			}
			else if (arrows < sortedZombies.top()->health)
			{
				sortedZombies.top()->health = sortedZombies.top()->health - arrows;
				arrows = 0;
				if (sortedZombies.top()->health == 0)
				{
					if (isVerbose)
					{
						cout << "Destroyed: " << sortedZombies.top()->name << " (distance: " << sortedZombies.top()->distance << ", speed: " << sortedZombies.top()->speed << ", health: " << sortedZombies.top()->health << ")" << "\n";
					}
					sortedZombies.top()->isActive = false;
					sortedZombies.top()->roundDied = count_in;
					sortedZombies.top()->age = count_in - sortedZombies.top()->roundBorn + 1;
					median_helper(sortedZombies.top()->age);
					//ages.push_back(sortedZombies.top()->age);
					deadZombies.push_back(sortedZombies.top());
					didZombieDie = true;
					if (sortedZombies.size() == 1)
					{
						lastJedi = sortedZombies.top()->name;
					}
					sortedZombies.pop();
				}
				return;
			}
			if (sortedZombies.empty())
			{
				if (!nextRound)
				{
					isGameWon = true;
					lastRound = count_in;
					return;
				}
				return;
			}
		}
		if (sortedZombies.empty())
		{
			if (!nextRound)
			{
				isGameWon = true;
				lastRound = count_in;
				return;
			}
			return;
		}
	}
	if (sortedZombies.empty())
	{
		if (!nextRound)
		{
			isGameWon = true;
			lastRound = count_in;
			return;
		}
		return;
	}
		
	//cout << "exited human attack" << "\n";
}

int get_median()
{
	if ((upper.size() + lower.size()) % 2 == 0)
	{
		return (upper.top() + lower.top()) / 2;
	}
	else if (upper.size() > lower.size())
	{
		return upper.top();
	}
	return lower.top();
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
	string key;
	int randZombies;
	int namedZombies;

	if (indicator != "---")
	{
		while (cin >> indicator)
		{
			//cout << indicator << "\n";
			if (indicator == "---")
			{
				cin >> key >> roundNum;
				break;
			}
		}
	}
		
	if (count == roundNum && indicator == "---")
	{
		cin >> key >> randZombies;
		cin >> key >> namedZombies;
		//cout << "round information: " << roundNum << randZombies << namedZombies << "\n";
		
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
			mostActiveZombies.push(&activeZombies[totalZombies]);
			leastActiveZombies.push(&activeZombies[totalZombies]);
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
			mostActiveZombies.push(&activeZombies[totalZombies]);
			leastActiveZombies.push(&activeZombies[totalZombies]);
			totalZombies++;
			//cout << activeZombies.size() << " " << sortedZombies.size() << "\n";
		}
		// THIS IF STATEMENT MIGHT BE THE CAUSE OF YOUR DOOM
	}
	// if a round is skipped
	else if (indicator == "---" && roundNum != count)
	{
		return;
	}
	if (cin >> indicator)
	{
		if (indicator == "---")
		{
			nextRound = true;
			cin >> key >> roundNum;
		}
	}
	else
	{
		nextRound = false;
	}

	//cout << "finished read_round" << "\n";
}

void update_age()
{
	for (size_t i = 0; i < activeZombies.size(); i++)
	{
		if (activeZombies[i].isActive)
		{
			activeZombies[i].age = lastRound - activeZombies[i].roundBorn + 1;
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
	if (!deadZombies.empty())
	{
		if (N < deadZombies.size())
		{
			min = N;
		}
		for (size_t i = 0; i < min; i++)
		{
			cout << deadZombies[i]->name << " " << i + 1 << "\n";
		}
	}

	size_t count1 = 1;

	cout << "Last zombies killed:" << "\n";
	if (!deadZombies.empty())
	{
		for (size_t i = deadZombies.size() - 1; i > 0 && N != count1 - 1; i--)
		{
			cout << deadZombies[i]->name << " " << N - count1 + 1 << "\n";
			count1++;
		}
		if (count1 < N + 1)
		{
			cout << deadZombies[0]->name << " " << 1 << "\n";
		}
	}

	cout << "Most active zombies:" << "\n";

	sort(activeZombies.begin(), activeZombies.end(), ageMore);

	count1 = 1;

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

