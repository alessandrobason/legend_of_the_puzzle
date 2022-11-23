#pragma once
#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

class Collision {
private:
	bool isenabled = true;

	sf::RectangleShape debug;

	void setupDebugBox(rectf r);
	
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
	Collision(rectf r, LAYER type);
	Collision(float x, float y, float w, float h, LAYER type);

	~Collision();

	void setEnabled(bool e) { isenabled = e; }
	void enable() { isenabled = true; }
	void disable() { isenabled = false; }
	bool isEnabled() { return isenabled; }

	void setDebugPosition(vec2 pos) { debug.setPosition(pos); }
	void setDebugSize(vec2 size) { debug.setSize(size); }
	void setDebugColor(sf::Color c) { debug.setOutlineColor(c); }

	bool Check_Collision(rectf r);

	void setPosition(vec2 pos);
	void setCenter(vec2 c);
	void moveCollision(vec2 mov);
	
	vec2 getCollisionSide(rectf r, vec2& oldVel);

	void drawDebug(sf::RenderWindow* w);

	LAYER collisionlayer;
	rectf rect;
	vec2 collision_offset;
	IntCircle circle;
};