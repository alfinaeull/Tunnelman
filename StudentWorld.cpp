#include "StudentWorld.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
// contains your StudentWorld class implementation 

// constructor
StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir)
{

}

// destructor
StudentWorld::~StudentWorld()
{
	cleanUp();
}

bool StudentWorld::digEarth(int i, int j)
{
	if (clearedEarth[i][j] == false)
	{
		earthField[i][j]->setVisible(false);
		clearedEarth[i][j] = true;
		return true;
	}
	return false;
}

void StudentWorld::validatePosition(int& x, int& y)
{
	while (true)
	{
		x = int(rand() % 61);
		y = int(rand() % 57);
		bool validated = true;

		for (Object* actor : actors)
		{
			float dist = measureDistance(x, y, actor->getX(), actor->getY());
			if (dist <= 6.0)
			{
				validated = false;
				break;
			}
		}
		if (validated)
		{
			return;
		}
	}
}

// init method must create the Tunnelman object and insert it into the oil field at the right
// starting location, Creates all of the oil field’s Earth objects and inserts them into a
// data structure that tracks active Earth
int StudentWorld::init()
{
	srand(time(0));

	int G = max(static_cast<int>(5 - getLevel() / 2), 2);
	int L = min(static_cast<int>(2 + getLevel()), 21);
	int B = min(static_cast<int>(getLevel() / 2 + 2), 9);
	barrelCount = L;

	for (int i = 0; i < L; i++)
	{
		int randx;
		int randy;
		validatePosition(randx, randy);

		actors.push_back(new Barrel(randx, randy, GraphObject::right, 1.0, 2, this));
	}
	for (int i = 0; i < G; i++)
	{
		int randx;
		int randy;
		validatePosition(randx, randy);

		actors.push_back(new Gold(randx, randy, GraphObject::right, 1.0, 2, this, false));
	}
	for (int i = 0; i < B; i++)
	{
		int randx;
		int randy;
		validatePosition(randx, randy);

		actors.push_back(new Boulder(randx, randy, GraphObject::down, 1.0, 1, this));
	}

	// initialize tunnelman
	tunnelman = new Tunnelman(30, 60, GraphObject::right, 1.0, 0, this);

	// create array of Earth field
	for (int i = 0; i < 64; i++) {

		for (int j = 0; j < 60; j++) {

			earthField[i][j] = new Earth(i, j, GraphObject::right, .25, 3, this);

		}
	}

	// add top four rows of walkable area to clearedEarth array
	for (int i = 0; i < 64; i++)
	{
		for (int j = 60; j < 64; j++)
		{
			clearedEarth[i][j] = true;
		}
	}

	// clear vertical shaft down Earth field
	for (int i = 30; i < 34; i++) {

		for (int j = 4; j < 60; j++) {

			earthField[i][j]->setVisible(false);
			clearedEarth[i][j] = true;
		}

	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::setDisplayText()
{
	int level = getLevel();
	int barrelsLeft = barrelCount;
	int score = getScore();
	int gold = tunnelman->getGoldCount();
	int sonars = tunnelman->getSonarCount();
	int squirts = tunnelman->getSquirtCount();

	string s = "Level: " + to_string(level) + " Barrels Left: " + to_string(barrelsLeft) + " Score: " + to_string(score) + " Gold: " +
		to_string(gold) + " Sonars: " + to_string(sonars) + " Squirts: " + to_string(squirts);
	setGameStatText(s);
}

// move method must, during each tick, ask your Tunnelman object to do something
int StudentWorld::move()
{
	bool tunnelman_alive = true;

	if (tunnelman_alive == false) {
		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	int G = getLevel() * 25 + 300; // Chance of spawning sonar kit or water pool
	if (rand() % G == 1) // spawn goodie
	{
		if (rand() % 5 == 1) // spawn a sonar
		{
			actors.push_back(new Sonar(0, 60, GraphObject::right, 1.0, 2, this));
		}
		else
		{
			//spawn a water pool
		}
	}

	setDisplayText();
	showObjectsNearPlayer(4);
	pickupObjectsNearPlayer();

	tunnelman->doSomething();
	for (auto actor : actors)
	{
		if (actor->isAlive())
		{
			actor->doSomething();
		}
	}

	removeDeadGameObjects();

	if (barrelCount == 0)
	{
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}

	return GWSTATUS_CONTINUE_GAME;
}

float StudentWorld::measureDistance(int x1, int y1, int x2, int y2)
{
	float xdist = static_cast<float>(abs(x2 - x1));
	float ydist = static_cast<float>(abs(y2 - y1));
	float totaldist = sqrt((xdist * xdist) + (ydist * ydist));
	return totaldist;
}

void StudentWorld::removeDeadGameObjects()
{
	if (actors.empty())
	{
		return;
	}
	auto it = actors.begin();
	while (it != actors.end())
	{
		if ((*it)->isAlive() == false)
		{
			auto temp = *it;
			it = actors.erase(it);
			delete (temp);
			continue;
		}
		it++;
	}
}

// TODO: refactor this so that the proximity checking can be used for gold and barrels on radar scan
//       this potentially also applies to checking to make sure objects don't spawn to close to one another.
void StudentWorld::showObjectsNearPlayer(int revealed)
{
	int tx = tunnelman->getX();
	int ty = tunnelman->getY();

	for (Object* actor : actors)
	{
		if (actor->canBeRevealed() && actor->isAlive())
		{
			int bx = actor->getX();
			int by = actor->getY();
			float dist = measureDistance(tx, ty, bx, by);
			if (dist <= revealed)
			{
				actor->setVisible(true);
			}
		}
	}
}

void StudentWorld::pickupObjectsNearPlayer()
{
	int tx = tunnelman->getX();
	int ty = tunnelman->getY();

	for (Object* actor : actors)
	{
		if (actor->canBeRevealed() && actor->isAlive())
		{
			int bx = actor->getX();
			int by = actor->getY();
			float dist = measureDistance(tx, ty, bx, by);
			if (dist <= 3.0)
			{
				int itemId = dynamic_cast<Goodie*>(actor)->pickupItem();
				if (itemId == TID_BARREL)
					barrelCount--;
				if (itemId == TID_GOLD)
					tunnelman->incrementGoldCount();
				if (itemId == TID_SONAR)
					tunnelman->incrementSonarCount();
			}
		}
	}
}

void StudentWorld::spawnGold()
{
	actors.push_back(new Gold(tunnelman->getX(), tunnelman->getY(), GraphObject::right, 1.0, 2, this, true));
}

void StudentWorld::spawnSquirt()
{
	GraphObject::Direction dir = tunnelman->getDirection();
	int spawnx = tunnelman->getX();
	int spawny = tunnelman->getY();
	switch (dir)
	{
	case GraphObject::up:
		spawny += 4;
		break;
	case GraphObject::down:
		spawny -= 4;
		break;
	case GraphObject::right:
		spawnx += 4;
		break;
	case GraphObject::left:
		spawnx -= 4;
		break;
	}

	if (spawnx < 0 || spawnx > 60 || spawny > 60 || spawny < 0)
	{
		return;
	}

	for (int i = spawnx; i < spawnx + 4; i++) {
		for (int j = spawny; j < spawny + 4; j++) {
			if (this->clearedEarth[i][j] == false)
			{
				return;
			}
		}
	}

	actors.push_back(new Squirt(spawnx, spawny, dir, 1.0, 1, this));
}

// cleanUp method must free any dynamically allocated data that was allocated during calls to the
// init() method or the move() method (will likely do the same thing as the destructor)
void StudentWorld::cleanUp()
{
	// clear tunnelman
	delete tunnelman;

	// clear earth field
	for (int i = 0; i < 64; i++) {

		for (int j = 0; j < 60; j++) {

			delete earthField[i][j];
			clearedEarth[i][j] = false;
		}
	}

	// clear actors
	for (auto it = actors.begin(); it != actors.end(); ++it)
	{
		delete* it;  // Delete the object pointed to by the iterator
	}
	actors.clear();
}
