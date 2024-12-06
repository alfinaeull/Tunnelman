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

	public:
		// constructor
		Tunnelman(int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* sw);

		// destructor
		~Tunnelman();

		// limited doSomething method
		void doSomething();

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

public:
	// constructor
	Gold(int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* w);

	// destructor
	~Gold();

	bool canBeRevealed() override { return true; }

	int pickupItem();

	void doSomething();
};

#endif // ACTOR_H_
