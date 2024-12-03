#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
// contains base, Earth, and Tunnelman class declarations
// and necessary constants

class Object : public GraphObject {
	private:


	public:
		// constructor
		Object(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0)
			: GraphObject(imageID, startX, startY, dir, size, depth)
		{
			setVisible(true);
		}
		
		// destructor
		~Object()
		{

		}

		// virtual method doSomething
		virtual void doSomething() = 0;


};


class Earth : public Object {
	private:
		
	
	public:
		// constructor
		Earth(int startX, int startY, Direction dir = right, double size = .25, unsigned int depth = 3)
			: Object(TID_EARTH, startX, startY, dir, size, depth)
		{
			
		}

		// destructor
		~Earth()
		{

		}
};


class Tunnelman : public Object {
	private:


	public:
		// constructor
		Tunnelman(int startX = 30, int startY = 60, Direction dir = right, double size = 1.0, unsigned int depth = 0)
			: Object(TID_PLAYER, startX, startY, dir, size, depth)
		{

		}

		// destructor
		~Tunnelman()
		{

		}

		// limited doSomething method
		void doSomething()
		{
			
		}

};






#endif // ACTOR_H_
