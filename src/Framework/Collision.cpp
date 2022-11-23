#include "Collision.h"
#include <iostream>

Collision::Collision() {
	rect = rectf(0, 0, 0, 0);
	collision_offset = vec2();
	setupDebugBox(rect);
}

Collision::Collision(rectf r, LAYER t) {
	collisionlayer = t;
	rect = r;
	collision_offset = vec2(r.left, r.top);
	setupDebugBox(rect);
}

Collision::Collision(float x, float y, float w, float h, LAYER t) {
	collisionlayer = t;
	rect = rectf(x, y, w, h);
	collision_offset = vec2(x, y);
	setupDebugBox(rect);
}

void Collision::setupDebugBox(rectf r) {
	debug.setPosition(vec2(r.left + 1, r.top + 1));
	debug.setSize(vec2(r.width - 2, r.height - 2));
	debug.setOutlineColor(sf::Color::Red);
	debug.setOutlineThickness(1.f);
	debug.setFillColor(sf::Color::Transparent);
}

Collision::~Collision() {}

//////////////////////////////////////////////////

bool Collision::Check_Collision(rectf r) {
	return rect.intersects(r) && r!=rect && isenabled;
}

vec2 Collision::getCollisionSide(rectf r, vec2& oldVel) {
	vec2 reverseVel = vec2(0, 0);

	bool left = false, right = false, top = false, bottom = false;
	
	if (oldVel.x > 0) left = true;
	else if (oldVel.x < 0) right = true;
	if (oldVel.y > 0) top = true;
	else if (oldVel.y < 0) bottom = true;

	//top = true; left = true; bottom = true; right = true;
	
	// if left side is after half of rectangle (coming from RIGHT)
	if (rect.left >= r.left + (r.width / 2) && right) {
		reverseVel.x = r.left + r.width - rect.left;
	}
	// if left side is before half of rectangle (coming from LEFT)
	else if(rect.left <= r.left + (r.width / 2) && left) {
		reverseVel.x -= rect.left + rect.width - r.left;
	}
	// if top side is after half of rectangle (coming from BOTTOM)
	if (rect.top >= r.top + (r.height/2) && bottom) {
		reverseVel.y = r.top + r.height - rect.top;
	}
	// if top side is after half of rectangle (coming from TOP)
	else if(rect.top <= r.top + (r.height / 2) && top){
		reverseVel.y -= rect.top + rect.height - r.top;
	}

	if (std::abs(reverseVel.x) > 1)		reverseVel.x = 0;
	else if(std::abs(reverseVel.y) > 1)	reverseVel.y = 0;
	
	return reverseVel;
}

void Collision::setPosition(vec2 pos) {
	rect.left = pos.x;
	rect.top = pos.y;
	debug.setPosition(vec2(rect.left + 1, rect.top + 1));
}

void Collision::setCenter(vec2 c) {
	rect.left = c.x - rect.width / 2;
	rect.top = c.y - rect.height / 2;
	debug.setPosition(vec2(rect.left + 1, rect.top + 1));
}

void Collision::moveCollision(vec2 mov) {
	rect.left += mov.x;
	rect.top += mov.y;
	debug.setPosition(vec2(rect.left + 1, rect.top + 1));
}

void Collision::drawDebug(sf::RenderWindow* w) {
	//debug.setPosition(vec2(rect.left, rect.top));
	w->draw(debug);
}
