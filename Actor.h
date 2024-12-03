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

#endif // ACTOR_H_
