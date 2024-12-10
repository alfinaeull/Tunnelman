#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
// contains base, Earth, and Tunnelman class declarations
// and necessary constants


class Object : public GraphObject {
private:
	StudentWorld* world;
	bool alive = true;
protected:
	void setState(bool state);
public:
	// constructor
	Object(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* w);

	// destructor
	~Object();

	// used to identify classes gold and barrel which are not initially visible to player
	virtual bool canBeRevealed() { return false; }

	// returns pointer to StudentWorld
	StudentWorld* getWorld() const;

	// virtual method doSomething
	virtual void doSomething() = 0;

	bool isAlive();
};


class Earth : public Object {
private:


public:
	// constructor
	Earth(int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* sw);

	// destructor
	~Earth();

	void doSomething();

};


class Tunnelman : public Object {
private:
	int currX;
	int currY;
	int goldCount;
	int sonarCount = 1;
	int squirtCount = 5;

public:
	// constructor
	Tunnelman(int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* sw);

	// destructor
	~Tunnelman();

	int getGoldCount();

	void incrementGoldCount();

	int getSonarCount();

	void incrementSonarCount();

	int getSquirtCount();

	// doSomething method
	void doSomething();

	void dropGold();

	void useSonar();

	void shootSquirt();
};

class Squirt : public Object {
private:
	int travelDistance = 4;
public:
	// constructor
	Squirt(int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* sw);

	// destructor
	~Squirt();

	void doSomething();
};

class Boulder : public Object {
private:
	std::string state;
	int tick_counter;
	int currX;
	int currY;

public:

	// constructor
	Boulder(int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* sw);

	// destructor
	~Boulder();

	// doSomething method
	void doSomething();

	bool checkForBoulder();

};



// Base class for pickups; gold, barrel, water, sonar
class Goodie : public Object
{
private:
	int x;
	int y;
public:
	// constructor
	Goodie(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* w);

	// destructor
	~Goodie();

	bool canBeRevealed() override { return true; }

	virtual int pickupItem() = 0;
};

class Barrel : public Goodie
{
private:

public:
	// constructor
	Barrel(int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* w);

	//destructor 
	~Barrel();

	int pickupItem();

	void doSomething();
};

class Gold : public Goodie
{
private:
	int timer = 0;
	bool createdByPlayer;
public:
	// constructor
	Gold(int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* w, bool createdByPlayer);

	// destructor
	~Gold();

	bool canBeRevealed() override;

	int pickupItem();

	void doSomething();
};

class Sonar : public Goodie
{
private:
	int timer = 0;
public:
	// constructor
	Sonar(int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* w);

	// destructor
	~Sonar();

	int pickupItem();

	void doSomething();
};


// base class for RegularProtestor and HardcoreProtestor
class Protestor : public Object {
private:
	int numSquaresToMoveInCurrentDirection;

	int hit_pts;

	std::string state;

public:
	// constructor
	Protestor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* w);

	// destructor
	~Protestor();

	virtual void doSomething();


};

#endif // ACTOR_H_
