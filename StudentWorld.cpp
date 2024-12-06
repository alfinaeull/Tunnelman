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

// init method must create the Tunnelman object and insert it into the oil field at the right
// starting location, Creates all of the oil field’s Earth objects and inserts them into a
// data structure that tracks active Earth
int StudentWorld::init()
{
	srand(time(0));

	int L = min(static_cast<int>(2 + getLevel()), 21);
	barrelCount = L;

	// TODO: make sure barrels can't spawn too close to one another
	for (int i = 0; i < L; i++)
	{
		int randx = int(rand() % 61);
		int randy = int(rand() % 57);
		actors.push_back(new Barrel(randx, randy, GraphObject::right, 1.0, 2, this));
	}

	// initialize tunnelman
	tunnelman = new Tunnelman(30, 60, GraphObject::right, 1.0, 0, this);

	// create array of Earth field
	for (int i = 0; i < 64; i++) {

		for (int j = 0; j < 60; j++) {

			earthField[i][j] = new Earth(i, j, GraphObject::right, .25, 3, this);

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
	int level = getLevel() + 1;
	int barrelsLeft = barrelCount;
	int score = getScore();

	string s = "Level: " + to_string(level) + " Barrels Left: " + to_string(barrelsLeft) + " Score: " + to_string(score);
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

	setDisplayText();

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
void StudentWorld::showObjectsNearPlayer()
{
	int tx = tunnelman->getX();
	int ty = tunnelman->getY();

	for (Object* actor : actors)
	{
		if (dynamic_cast<Barrel*>(actor) != nullptr && actor->isAlive())
		{
			int bx = actor->getX();
			int by = actor->getY();
			float dist = measureDistance(tx, ty, bx, by);
			if (dist <= 3.0)
			{
				dynamic_cast<Barrel*>(actor)->pickupItem();
				barrelCount--;
			}
			else if (dist <= 4.0)
			{
				actor->setVisible(true);
			}
		}
	}
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
