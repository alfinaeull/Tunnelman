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
		Object(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0, StudentWorld* w = nullptr);
		
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
		Earth(int startX, int startY, Direction dir = right, double size = .25, unsigned int depth = 3, StudentWorld* sw = nullptr);

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
		Tunnelman(int startX = 30, int startY = 60, Direction dir = right, double size = 1.0, unsigned int depth = 0, StudentWorld* sw = nullptr);

		// destructor
		~Tunnelman();

		// limited doSomething method
		void doSomething();

};






#endif // ACTOR_H_
