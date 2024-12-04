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
StudentWorld* Object::getWorld()
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

	if (getWorld()->getKey(key) == true) {

		// if user hits a key

		switch (key)
		{
			// update Tunnelman location to target square as long as within oil field
			case KEY_PRESS_LEFT: // move player to the left
			{
				// update Tunnelman location to target square as long as within oil field
				if (currX != LEFT_EDGE)
				{
					currX--;
					moveTo(currX, currY);
				}
				break;
			}

			case KEY_PRESS_RIGHT: // move player to the right
			{
				if (currX != RIGHT_EDGE - TUNNELMAN_SIZE)
				{
					currX++;
					moveTo(currX, currY);
				}
				break;
			}

			case KEY_PRESS_UP: // move player up
			{
				if (currY != TOP_EDGE)
				{
					currY++;
					moveTo(currX, currY);
				}
				break;
			}

			case KEY_PRESS_DOWN: // move player down
			{
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
	for (int i = currX; i < currX + 4; i++) {

		for (int j = currY; j < currY + 4; j++) {

			if (i >= 0 && i < 64 && j >= 0 && j < 60) {

				getWorld()->digEarth(i, j);
			}

		}
	}

}
