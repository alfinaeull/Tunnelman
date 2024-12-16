#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
// contains implementations of base, Earth, and Tunnelman classes

const int RIGHT_EDGE = 64;
const int LEFT_EDGE = 0;
const int TOP_EDGE = 60;
const int BOTTOM_EDGE = 0;

const int TUNNELMAN_SIZE = 4;
const int GOLD_DESPAWN_TIMER = 100;

Object::Object(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0, StudentWorld* w = nullptr)
	: GraphObject(imageID, startX, startY, dir, size, depth)
{
	setVisible(true);
	world = w;
}

// destructor
Object::~Object()
{

}

// returns pointer to StudentWorld
StudentWorld* Object::getWorld() const
{
	return world;
}

void Object::setState(bool state)
{
	alive = state;
}

bool Object::isAlive()
{
	return alive;
}

// constructor
Earth::Earth(int startX, int startY, Direction dir = right, double size = .25, unsigned int depth = 3, StudentWorld* sw = nullptr)
	: Object(TID_EARTH, startX, startY, dir, size, depth, sw)
{

}

// destructor
Earth::~Earth()
{

}

void Earth::doSomething()
{

}

// constructor
Boulder::Boulder(int startX, int startY, Direction dir = down, double size = 1.0, unsigned int depth = 1, StudentWorld* sw = nullptr)
	: Object(TID_BOULDER, startX, startY, dir, size, depth, sw)
{
	state = "stable";
	tick_counter = 0;

	currX = startX;
	currY = startY;
}

// destructor
Boulder::~Boulder()
{

}

// doSomething method
void Boulder::doSomething()
{
	// check if alive
	if (isAlive() != true) {
		return;
	}

	// increment tick_counter if it's started
	if (tick_counter > 0) {
		tick_counter++;
	}

	if (state == "stable") {

		// don't do anything, just return if there is Earth 4 squares or less beneath boulder
		for (int i = currX; i < currX + 4; i++) {

			if (i < 64 && currY - 1 >= 0 && getWorld()->existingTerrain(i, currY - 1, 1, 1, "Earth")) {

				return;
			}
		}

		// but if any of the 4 squares beneath don't have Earth, transition into waiting state
		state = "waiting";
		tick_counter++; // start the tick_counter
	}

	// once tick_counter reaches 30, transition into falling state
	if (tick_counter == 30) {

		state = "falling";
		getWorld()->removeBoulder(getX(), getY());
		getWorld()->playSound(SOUND_FALLING_ROCK);
	}

	// if boulder is in falling state, it moves down 1 every tick until
	// a) hits bottom of oil field
	// b) runs into the top of another boulder
	// c) runs into earth
	// at any of those points, set state to dead
	if (state == "falling") {

		if (currY - 1 < 0 || getWorld()->existingTerrain(currX, currY - 1, 4, 1, "Boulder") || getWorld()->existingTerrain(currX, currY - 1, 4, 1, "Earth")) {

			state = "dead";
		}
		else {
			// if none of the conditions are met, move down 1 tick
			moveTo(currX, currY - 1);
			currY--;
		}

		// code for boulder interactions with protestors and tunnelman
	}

	if (state == "dead") {
		setState(false);
	}
}

// constructor
Tunnelman::Tunnelman(int startX = 30, int startY = 60, Direction dir = right, double size = 1.0, unsigned int depth = 0, StudentWorld* sw = nullptr)
	: Object(TID_PLAYER, startX, startY, dir, size, depth, sw)
{
	currX = startX;
	currY = startY;
}

// destructor
Tunnelman::~Tunnelman()
{

}

// limited doSomething method
void Tunnelman::doSomething()
{
	int key;
	GraphObject::Direction lastDirection = getDirection();

	if (getWorld()->getKey(key) == true) {

		// if user hits a key

		switch (key)
		{
			// update Tunnelman location to target square as long as within oil field
		case KEY_PRESS_LEFT: // move player to the left
		{
			setDirection(left);
			if (lastDirection != left)
			{
				break;
			}
			// update Tunnelman location to target square as long as within oil field
			setDirection(left);
			if (currX != LEFT_EDGE)
			{
				if (getWorld()->existingTerrain(currX - 1, currY, 1, 4, "Boulder"))
				{
					break;
				}
				currX--;
				moveTo(currX, currY);
			}
			break;
		}

		case KEY_PRESS_RIGHT: // move player to the right
		{
			setDirection(right);
			if (lastDirection != right)
			{
				break;
			}
			if (currX != RIGHT_EDGE - TUNNELMAN_SIZE)
			{
				if (getWorld()->existingTerrain(currX + TUNNELMAN_SIZE, currY, 1, 4, "Boulder"))
				{
					break;
				}
				currX++;
				moveTo(currX, currY);
			}
			break;
		}

		case KEY_PRESS_UP: // move player up
		{
			setDirection(up);
			if (lastDirection != up)
			{
				break;
			}
			if (currY != TOP_EDGE)
			{
				if (getWorld()->existingTerrain(currX, currY + TUNNELMAN_SIZE, 4, 1, "Boulder"))
				{
					break;
				}
				currY++;
				moveTo(currX, currY);
			}
			break;
		}

		case KEY_PRESS_DOWN: // move player down
		{
			setDirection(down);
			if (lastDirection != down)
			{
				break;
			}
			if (currY != BOTTOM_EDGE)
			{
				if (getWorld()->existingTerrain(currX, currY - 1, 4, 1, "Boulder"))
				{
					break;
				}
				currY--;
				moveTo(currX, currY);
			}
			break;
		}
		case KEY_PRESS_TAB:
		{
			dropGold();
			break;
		}
		case 'z':
		case 'Z':
		{
			useSonar();
			break;
		}
		case KEY_PRESS_SPACE:
		{
			shootSquirt();
			break;
		}
		}
	}

	// remove any Earth objects that overlap with Tunnelman's 4x4 image
	bool earthRemoved = false;
	for (int i = currX; i < currX + 4; i++) {

		for (int j = currY; j < currY + 4; j++) {

			if (i >= 0 && i < 64 && j >= 0 && j < 60) {
				if (earthRemoved)
					getWorld()->digEarth(i, j);
				else
					earthRemoved = getWorld()->digEarth(i, j);
			}

		}
	}
	if (earthRemoved)
	{
		getWorld()->playSound(SOUND_DIG);
	}
}

int Tunnelman::getHealth()
{
	return hp;
}

void Tunnelman::decrementHealth(int hpLost)
{
	hp -= hpLost;
	getWorld()->playSound(SOUND_PLAYER_ANNOYED);
}

int Tunnelman::getGoldCount()
{
	return goldCount;
}

void Tunnelman::dropGold()
{
	if (goldCount > 0)
	{
		goldCount--;
		getWorld()->spawnGold();
	}
}

void Tunnelman::incrementGoldCount()
{
	goldCount++;
}

int Tunnelman::getSonarCount()
{
	return sonarCount;
}

void Tunnelman::incrementSonarCount()
{
	sonarCount++;
}

void Tunnelman::useSonar()
{
	if (sonarCount == 0)
	{
		return;
	}
	this->getWorld()->showObjectsNearPlayer(12);
	sonarCount--;
}

int Tunnelman::getSquirtCount()
{
	return squirtCount;
}

void Tunnelman::shootSquirt()
{
	if (squirtCount == 0)
	{
		return;
	}
	this->getWorld()->spawnSquirt();
	this->getWorld()->playSound(SOUND_PLAYER_SQUIRT);
	squirtCount--;
}

Squirt::Squirt(int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* sw = nullptr)
	: Object(TID_WATER_SPURT, startX, startY, dir, size, depth, sw)
{

}

Squirt::~Squirt()
{

}

void Squirt::doSomething()
{
	if (!(isAlive()))
	{
		return;
	}
	GraphObject::Direction dir = getDirection();
	switch (dir)
	{
	case GraphObject::up:
		if (getY() + 1 > 60)
		{
			setState(false);
			break;
		}
		moveTo(getX(), getY() + 1);
		break;
	case GraphObject::down:
		if (getY() - 1 < 0)
		{
			setState(false);
			break;
		}
		moveTo(getX(), getY() - 1);
		break;
	case GraphObject::right:
		if (getX() + 1 > 64)
		{
			setState(false);
			break;
		}
		moveTo(getX() + 1, getY());
		break;
	case GraphObject::left:
		if (getX() - 1 < 0)
		{
			setState(false);
			break;
		}
		moveTo(getX() - 1, getY());
		break;
	}
	travelDistance--;
	if (travelDistance == 0)
	{
		setState(false);
	}
}

Goodie::Goodie(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* w = nullptr)
	: Object(imageID, startX, startY, dir, size, depth, w)
{

}

Goodie::~Goodie()
{

}

Barrel::Barrel(int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* w = nullptr)
	: Goodie(TID_BARREL, startX, startY, dir, size, depth, w)
{
	setVisible(false);
}

Barrel::~Barrel()
{

}

int Barrel::pickupItem()
{
	this->getWorld()->increaseScore(1000);
	this->getWorld()->playSound(SOUND_FOUND_OIL);
	setState(false);
	return TID_BARREL;
}

void Barrel::doSomething()
{
	if (!isAlive())
	{
		return;
	}
}

Gold::Gold(int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* w = nullptr, bool createdByPlayer = false)
	: Goodie(TID_GOLD, startX, startY, dir, size, depth, w)
{
	this->createdByPlayer = createdByPlayer;
	if (createdByPlayer != true)
	{
		setVisible(false);
	}
}

Gold::~Gold()
{

}

bool Gold::canBeRevealed()
{
	if (createdByPlayer == true)
	{
		return false;
	}
	return true;
}

int Gold::pickupItem()
{
	this->getWorld()->increaseScore(10);
	this->getWorld()->playSound(SOUND_GOT_GOODIE);
	setState(false);
	return TID_GOLD;
}

void Gold::doSomething()
{
	if (createdByPlayer == true)
	{
		timer++;
	}
	if (timer >= GOLD_DESPAWN_TIMER)
	{
		setState(false);
	}
	if (!isAlive())
	{
		return;
	}
}

Sonar::Sonar(int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* w)
	: Goodie(TID_SONAR, startX, startY, dir, size, depth, w)
{

}

Sonar::~Sonar()
{

}

int Sonar::pickupItem()
{
	this->getWorld()->increaseScore(75);
	this->getWorld()->playSound(SOUND_GOT_GOODIE);
	setState(false);
	return TID_SONAR;
}

void Sonar::doSomething()
{
	timer++;
	int sonarTime = std::max(100, int(300 - 10 * this->getWorld()->getLevel()));
	if (timer >= sonarTime)
	{
		setState(false);
	}
	if (!isAlive())
	{
		return;
	}
}

Protester::~Protester()
{

}

RegProtester::RegProtester(int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* w)
	: Protester(TID_PROTESTER, startX, startY, dir, size, depth, w)
{

}

RegProtester::~RegProtester()
{

}

void RegProtester::doSomething()
{
	// check if protester is alive
	if (!isAlive())
	{
		return;
	}
	restTimer++;
	// std::max(50, int(100 - getWorld()->getLevel() * 10)); 
	// check if protester should be resting
	int ticksToWaitBetweenMoves = std::max(0, int(3 - getWorld()->getLevel() / 4));
	if (restTimer < ticksToWaitBetweenMoves)
	{
		return;
	}
	else
	{
		restTimer = 0;
		shoutCooldown++;
	}

	int dir = getWorld()->protesterLineOfSight(getX(), getY());
	if (dir != GraphObject::none)
	{
		if (dir == GraphObject::up)
		{
			setDirection(GraphObject::up);
			moveTo(getX(), getY() + 1);
			stepCount++;
			getWorld()->updateDistanceMap(getX(), getY(), stepCount);
			return;
		}
		if (dir == GraphObject::down)
		{
			setDirection(GraphObject::down);
			moveTo(getX(), getY() - 1);
			stepCount++;
			getWorld()->updateDistanceMap(getX(), getY(), stepCount);
			return;
		}
		if (dir == GraphObject::right)
		{
			setDirection(GraphObject::right);
			moveTo(getX() + 1, getY());
			stepCount++;
			getWorld()->updateDistanceMap(getX(), getY(), stepCount);
			return;
		}
		if (dir == GraphObject::left)
		{
			setDirection(GraphObject::left);
			moveTo(getX() - 1, getY());
			stepCount++;
			getWorld()->updateDistanceMap(getX(), getY(), stepCount);
			return;
		}
	}
	if (shoutCooldown >= 15)
	{
		if (getWorld()->shoutAtTunnelman(getX(), getY()) == true)
		{
			getWorld()->playSound(SOUND_PROTESTER_YELL);
			shoutCooldown = 0;
		}
		return;
	}
}