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

public:
	// constructor
	Tunnelman(int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* sw);

	// destructor
	~Tunnelman();

	// limited doSomething method
	void doSomething();

	int getGoldCount();

	void incrementGoldCount();

	// destructor
	~Tunnelman();

	// doSomething method
	void doSomething();

	void dropGold();
};


class Boulder : public Object {
private:
	std::string state;

public: 

	// constructor
	Boulder(int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* sw);

	// destructor
	~Boulder();

	// doSomething method
	void doSomething();

	std::string getState();


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

#endif // ACTOR_H_
