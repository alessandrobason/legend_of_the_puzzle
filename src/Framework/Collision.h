#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#include <raylib.h>

#include "../maths.h"

class Collision {
private:
	bool isenabled = true;

	rect debug;
	Color color;

	void setupDebugBox(rect r);
	
	struct IntCircle {
		vec2i pos;
		int radius;
	};

public:
	enum class LAYER {
		ALL,
		NONE,
		WATER,
		OBJECT,
		PLAYER,
		ENEMY
	};
	
	Collision();
	Collision(rect r, LAYER type);
	Collision(float x, float y, float w, float h, LAYER type);

	~Collision();

	void setEnabled(bool e) { isenabled = e; }
	void enable() { isenabled = true; }
	void disable() { isenabled = false; }
	bool isEnabled() { return isenabled; }

	void setDebugPosition(vec2 pos) { debug.pos = pos; }
	void setDebugSize(vec2 size) { debug.size = size; }
	void setDebugColor(Color c) { color = c; }

	bool Check_Collision(rect r);

	void setPosition(vec2 pos);
	void setCenter(vec2 c);
	void moveCollision(vec2 mov);
	
	vec2 getCollisionSide(rect r, vec2& oldVel);

	void drawDebug();

	LAYER collisionlayer;
	rect box;
	vec2 collision_offset;
	IntCircle circle;
};