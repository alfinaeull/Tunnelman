#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <functional>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
// contains your StudentWorld class declaration


// forward declaration for Earth and Tunnelman
class Tunnelman;
class Earth;
class Object;

class StudentWorld : public GameWorld
{
private:
	Tunnelman* tunnelman;
	Earth* earthField[64][60];
	bool clearedEarth[64][64] = { false };
	bool isBoulder[64][64] = { false };
	std::vector<Object*> actors;
	int barrelCount;
	int distanceMap[64][64] = { 100 };

	float measureDistance(int x1, int y1, int x2, int y2);
	void removeDeadGameObjects();
	void setDisplayText();
	void validatePosition(int& x, int& y);

public:
	// constructor
	StudentWorld(std::string assetDir);

	// destructor
	~StudentWorld();

	bool digEarth(int i, int j);

	void removeBoulder(int i, int j);

	bool existingTerrain(int i, int j, int width, int height, std::string s);

	// init method must create the Tunnelman object and insert it into the oil field at the right
	// starting location, Creates all of the oil field’s Earth objects and inserts them into a
	// data structure that tracks active Earth
	virtual int init();

	// move method must, during each tick, ask your Tunnelman object to do something
	virtual int move();

	void showObjectsNearPlayer(int revealed);

	void pickupObjectsNearPlayer();

	void spawnGold();

	void spawnSquirt();

	void updateDistanceMap(int i, int j, int steps);

	int protesterLineOfSight(int px, int py);

	// cleanUp method must free any dynamically allocated data that was allocated during calls to the
	// init() method or the move() method (will likely do the same thing as the destructor)
	virtual void cleanUp();
};

#endif // STUDENTWORLD_H_
