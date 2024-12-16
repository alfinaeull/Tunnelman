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

void StudentWorld::removeBoulder(int i, int j)
{
	for (int x = i; x < i + 4; x++) {
		for (int y = j; y < j + 4; y++) {
			isBoulder[x][y] = false;
		}
	}
}

bool StudentWorld::existingTerrain(int i, int j, int width, int height, std::string s)
{
	for (int x = i; x < i + width; x++)
	{
		for (int y = j; y < j + height; y++)
		{
			if (s == "Boulder" || s == "Any")
			{
				if (isBoulder[x][y] == true)
				{
					return true;
				}
			}
			if (s == "Earth" || s == "Any")
			{
				if (clearedEarth[x][y] == false)
				{
					return true;
				}
			}
		}
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

		if ((x > 26 && x < 34))
		{
			continue;
		}

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
	for (int i = 60; i < 64; i++)
	{
		for (int j = 60; j < 64; j++)
		{
			distanceMap[i][j] = 0;
		}
	}

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

	for (int i = 0; i < B; i++)
	{
		int randx;
		int randy;
		validatePosition(randx, randy);
		for (int i = randx; i < randx + 4; i++) {
			for (int j = randy; j < randy + 4; j++) {

				earthField[i][j]->setVisible(false);
				clearedEarth[i][j] = true;
				isBoulder[i][j] = true;
			}
		}
		actors.push_back(new Boulder(randx, randy, GraphObject::down, 1.0, 1, this));
	}

	// clear vertical shaft down Earth field
	for (int i = 30; i < 34; i++) {

		for (int j = 4; j < 60; j++) {

			earthField[i][j]->setVisible(false);
			clearedEarth[i][j] = true;
		}

	}

	actors.push_back(new RegProtester(60, 60, GraphObject::left, 1.0, 0, this));

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::setDisplayText()
{
	int level = getLevel();
	int health = tunnelman->getHealth();
	int barrelsLeft = barrelCount;
	int score = getScore();
	int gold = tunnelman->getGoldCount();
	int sonars = tunnelman->getSonarCount();
	int squirts = tunnelman->getSquirtCount();
	int lives = getLives();

	string s = "Level: " + to_string(level) + " Health: " + to_string(health * 10) + "% Barrels Left: " + to_string(barrelsLeft) + " Score: " + to_string(score) + " Gold: " +
		to_string(gold) + " Sonars: " + to_string(sonars) + " Squirts: " + to_string(squirts) + " Lives: " + to_string(lives);
	setGameStatText(s);
}

// move method must, during each tick, ask your Tunnelman object to do something
int StudentWorld::move()
{
	if (tunnelman->getHealth() <= 0) {
		decLives();
		playSound(SOUND_PLAYER_GIVE_UP);
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

// returns true if player took damage
bool StudentWorld::shoutAtTunnelman(int px, int py)
{
	int tx = tunnelman->getX();
	int ty = tunnelman->getY();
	if (measureDistance(px, py, tx, ty) <= 4)
	{
		tunnelman->decrementHealth(2);
		return true;
	}
	return false;
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

void StudentWorld::updateDistanceMap(int i, int j, int steps)
{
	if (distanceMap[i][j] > steps)
	{
		distanceMap[i][j] = steps;
	}
}

// returns the direction the protester should move in
int StudentWorld::protesterLineOfSight(int px, int py)
{
	int tx = tunnelman->getX();
	int ty = tunnelman->getY();

	if (measureDistance(tx, ty, px, py) <= 4.0)
	{
		return GraphObject::none;
	}

	if (tx == px)
	{
		if (ty > py)
		{
			if (existingTerrain(px, py, 4, ty - py, "Any"))
			{
				return GraphObject::none;
			}
			return GraphObject::up;
		}
		else if (ty < py)
		{
			if (existingTerrain(tx, ty, 4, py - ty, "Any"))
			{
				return GraphObject::none;
			}
			return GraphObject::down;
		}
	}
	else if (ty == py)
	{
		if (tx > px)
		{
			if (existingTerrain(px, py, tx - px, 4, "Any"))
			{
				return GraphObject::none;
			}
			return GraphObject::right;
		}
		else if (tx < px)
		{
			if (existingTerrain(tx, ty, px - tx, 4, "Any"))
			{
				return GraphObject::none;
			}
			return GraphObject::left;
		}
	}
	return GraphObject::none;
}

// Returns true if damage was successful
bool StudentWorld::processSquirtDamage(int sx, int sy)
{
	for (Object* actor : actors)
	{
		if (actor->canBeDamaged()) // tunnelman is not in this list
		{
			if (measureDistance(sx, sy, actor->getX(), actor->getY()) <= 3)
			{
				static_cast<Protester*>(actor)->decrementHealth(2);
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::processBoulderDamage(int bx, int by)
{
	for (Object* actor : actors)
	{
		if (actor->canBeDamaged()) // tunnelman is not in this list
		{
			if (measureDistance(bx, by, actor->getX(), actor->getY()) <= 4)
			{
				static_cast<Protester*>(actor)->decrementHealth(100);
				return true;
			}
		}
	}
	return false;
}

int StudentWorld::findExit(int px, int py)
{
	int lowestValue = 100;
	GraphObject::Direction dir = GraphObject::none;
	if (!existingTerrain(px, py + 4, 4, 1, "Any"))
	{
		for (int i = px; i < px + 4; i++)
		{
			if (distanceMap[i][py + 4] < lowestValue)
			{
				lowestValue = distanceMap[i][py + 4];
				dir = GraphObject::up;
			}
		}
	}
	if (!existingTerrain(px + 4, py, 1, 4, "Any"))
	{
		for (int j = py; j < py + 4; j++)
		{
			if (distanceMap[px + 4][j] < lowestValue)
			{
				lowestValue = distanceMap[px + 4][j];
				dir = GraphObject::right;
			}
		}
	}
	if (!existingTerrain(px, py - 1, 4, 1, "Any"))
	{
		for (int i = px; i < px + 4; i++)
		{
			if (distanceMap[i][py - 1] < lowestValue)
			{
				lowestValue = distanceMap[i][py - 1];
				dir = GraphObject::down;
			}
		}
	}
	if (!existingTerrain(px - 1, py, 1, 4, "Any"))
	{
		for (int j = py - 1; j < py; j++)
		{
			if (distanceMap[px - 1][j] < lowestValue)
			{
				lowestValue = distanceMap[px - 1][j];
				dir = GraphObject::left;
			}
		}
	}
	return dir;
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
