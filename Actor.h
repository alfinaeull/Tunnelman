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

	virtual void pickupItem() = 0;
};

class Barrel : public Goodie
{
private:

public:
	// constructor
	Barrel(int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* w);

	//destructor 
	~Barrel();

	void pickupItem();

	void doSomething();
};


#endif // ACTOR_H_
