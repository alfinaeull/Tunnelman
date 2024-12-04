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

	public:
		// constructor
		Object(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, StudentWorld* w);
		
		// destructor
		~Object();

		// returns pointer to StudentWorld
		StudentWorld* getWorld();

		// virtual method doSomething
		virtual void doSomething() = 0;

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



#endif // ACTOR_H_
