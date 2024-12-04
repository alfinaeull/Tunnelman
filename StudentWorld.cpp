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

void StudentWorld::digEarth(int i, int j)
{
	earthField[i][j]->setVisible(false);

}

// init method must create the Tunnelman object and insert it into the oil field at the right
// starting location, Creates all of the oil field’s Earth objects and inserts them into a
// data structure that tracks active Earth
int StudentWorld::init()
{
	// initialize tunnelman
	tunnelman = new Tunnelman(30, 60, GraphObject::right, 1.0, 0, this);

	// create array of Earth field
	for (int i = 0; i < 64; i++) {
		
		for (int j = 0; j < 60; j++) {

			earthField[i][j] = new Earth(i, j, GraphObject::right, .25, 3, this);

		}
	}

	return GWSTATUS_CONTINUE_GAME;
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
	
	tunnelman->doSomething();
	return GWSTATUS_CONTINUE_GAME;
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

		}
	}

}

