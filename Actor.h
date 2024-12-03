#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
// contains base, Earth, and Tunnelman class declarations
// and necessary constants

// forward declaration for StudentWorld ptr
class StudentWorld;


class Object : public GraphObject {
	private:
		StudentWorld* world;

	public:
		// constructor
		Object(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0, StudentWorld* w)
			: GraphObject(imageID, startX, startY, dir, size, depth)
		{
			setVisible(true);
			world = w;
		}
		
		// destructor
		~Object()
		{

		}

		// returns pointer to StudentWorld
		StudentWorld* getWorld()
		{
			return world;
		}

		// virtual method doSomething
		virtual void doSomething() = 0;


};


class Earth : public Object {
	private:
		
	
	public:
		// constructor
		Earth(int startX, int startY, Direction dir = right, double size = .25, unsigned int depth = 3, StudentWorld* sw)
			: Object(TID_EARTH, startX, startY, dir, size, depth, sw)
		{
			
		}

		// destructor
		~Earth()
		{

		}

};


class Tunnelman : public Object {
	private:
		int currX;
		int currY;

	public:
		// constructor
		Tunnelman(int startX = 30, int startY = 60, Direction dir = right, double size = 1.0, unsigned int depth = 0, StudentWorld* sw)
			: Object(TID_PLAYER, startX, startY, dir, size, depth, sw)
		{
			currX = startX;
			currY = startY;
		}

		// destructor
		~Tunnelman()
		{

		}

		// limited doSomething method
		void doSomething()
		{
			int key;

			if (getWorld()->getKey(key) == true) {

				// if user hits a key

				switch (key)
				{
					case KEY_PRESS_LEFT: // move player to the left
					{
						// update Tunnelman location to target square
						currX--;
						moveTo(currX, currY);
						break;
					}
					
					case KEY_PRESS_RIGHT: // move player to the right
					{
						currX++;
						moveTo(currX, currY);
						break;
					}
					
					case KEY_PRESS_UP: // move player up
					{
						currY++;
						moveTo(currX, currY);
						break;
					}
					
					case KEY_PRESS_DOWN: // move player down
					{
						currY--;
						moveTo(currX, currY);
						break;
					}

				}

			}

			// remove any Earth objects that overlap with Tunnelman's 4x4 image


		}

};






#endif // ACTOR_H_
