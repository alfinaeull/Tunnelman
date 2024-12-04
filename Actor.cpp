#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
// contains implementations of base, Earth, and Tunnelman classes

const int RIGHT_EDGE = 64;
const int LEFT_EDGE = 0;
const int TOP_EDGE = 60;
const int BOTTOM_EDGE = 0;

const int TUNNELMAN_SIZE = 4;

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
