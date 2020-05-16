// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#include <vector>
#include <iostream>
#include <stack>
#include <queue>
#include <deque>
#include <string>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <getopt.h>

using namespace std;

class zombie
{
private:
	

public:
	string name;
	int distance;
	int speed;
	int health;
	int roundBorn;
	int roundDied;
	int age = 0;
	int ETA;
	bool isActive = true;

	zombie(string name_in, int dist_in, int speed_in, int hp_in, int round_in)
		: name(name_in), distance(dist_in), speed(speed_in), health(hp_in), roundBorn(round_in) {}
	
	void move(bool &isDead, bool isVerbose, string &chadZombie)
	{
		if (isActive)
		{
			distance = max(0, distance - speed);
			ETA = distance / speed;
			age++;

			if (isVerbose)
			{
				cout << "Moved: " << name << " (distance: " << distance << ", speed: " << speed << ", health: " << health << ")" << endl;
			}
			if (!isDead)
			{
				chadZombie = name;
			}
			if (distance == 0)
			{
				isDead = true;
			}
		}
	}
};

