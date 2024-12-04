#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Object::Object(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* w)
	: GraphObject(imageID, startX, startY, dir, size, depth)
{
	setVisible(true);
	world = w;
}

Object::~Object()
{

}

StudentWorld* Object::getWorld()
{
	return world;
}


Earth::Earth(int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* sw)
	: Object(TID_EARTH, startX, startY, dir, size, depth, sw)
{

}


Earth::~Earth()
{

}

void Earth::doSomething()
{

}

Tunnelman::Tunnelman(int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* sw)
	: Object(TID_PLAYER, startX, startY, dir, size, depth, sw)
{
	currX = startX;
	currY = startY;
}

Tunnelman::~Tunnelman()
{

}

void Tunnelman::doSomething()
{
	int key;

	if (getWorld()->getKey(key) == true) {

		// if user hits a key

		switch (key)
		{
		case KEY_PRESS_LEFT: // move player to the left
		{
			// update Tunnelman location to target square
			currX--;
			moveTo(currX, currY);
			break;
		}

		case KEY_PRESS_RIGHT: // move player to the right
		{
			currX++;
			moveTo(currX, currY);
			break;
		}

		case KEY_PRESS_UP: // move player up
		{
			currY++;
			moveTo(currX, currY);
			break;
		}

		case KEY_PRESS_DOWN: // move player down
		{
			currY--;
			moveTo(currX, currY);
			break;
		}

		}

	}

	// remove any Earth objects that overlap with Tunnelman's 4x4 image


}