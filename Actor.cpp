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
	
}

// destructor
Boulder::~Boulder()
{

}

// doSomething method
void Boulder::doSomething()
{

}

std::string Boulder::getState()
{
	return state;
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

int Tunnelman::getGoldCount()
{
	return goldCount;
}

void Tunnelman::dropGold()
{
	if (goldCount > 0)
	{
		goldCount--;
		getWorld()->spawnGold(this);
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
	this->getWorld()->showObjectsNearPlayer(12);
	sonarCount--;
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
	this->getWorld()->showObjectsNearPlayer(4);
	this->getWorld()->pickupObjectsNearPlayer();
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
	this->getWorld()->showObjectsNearPlayer(4);
	this->getWorld()->pickupObjectsNearPlayer();
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
	this->getWorld()->showObjectsNearPlayer(4);
	this->getWorld()->pickupObjectsNearPlayer();
}