#include "Collision.h"
#include <iostream>

Collision::Collision() {
	box = { 0, 0, 0, 0 };
	collision_offset = vec2();
	setupDebugBox(box);
}

Collision::Collision(rect r, LAYER t) {
	collisionlayer = t;
	box = r;
	collision_offset = r.pos;
	setupDebugBox(box);
}

Collision::Collision(float x, float y, float w, float h, LAYER t) {
	collisionlayer = t;
	box = { x, y, w, h };
	collision_offset = { x, y };
	setupDebugBox(box);
}

void Collision::setupDebugBox(rect r) {
	debug.pos = r.pos + 1.f;
	debug.size = r.size - 2.f;
	color = RED;
}

Collision::~Collision() {}

//////////////////////////////////////////////////

bool Collision::Check_Collision(rect r) {
	return isenabled && CheckCollisionRecs(box, r);
}

vec2 Collision::getCollisionSide(rect r, vec2& oldVel) {
	vec2 reverseVel = vec2(0, 0);

	bool left = false, right = false, top = false, bottom = false;
	
	if (oldVel.x > 0) left = true;
	else if (oldVel.x < 0) right = true;
	if (oldVel.y > 0) top = true;
	else if (oldVel.y < 0) bottom = true;

	//top = true; left = true; bottom = true; right = true;
	
	// if left side is after half of rectangle (coming from RIGHT)
	if (box.x >= r.x + (r.w / 2.f) && right) {
		reverseVel.x = r.x + r.w - box.x;
	}
	// if left side is before half of rectangle (coming from LEFT)
	else if (box.x <= r.x + (r.w / 2.f) && left) {
		reverseVel.x -= box.x + box.w - r.x;
	}
	// if top side is after half of rectangle (coming from BOTTOM)
	if (box.y >= r.y + (r.h / 2.f) && bottom) {
		reverseVel.y = r.y + r.h - box.y;
	}
	// if top side is after half of rectangle (coming from TOP)
	else if(box.y <= r.y + (r.h / 2) && top){
		reverseVel.y -= box.y + box.h - r.y;
	}

	if (std::abs(reverseVel.x) > 1)		reverseVel.x = 0;
	else if(std::abs(reverseVel.y) > 1)	reverseVel.y = 0;
	
	return reverseVel;
}

void Collision::setPosition(vec2 pos) {
	box.pos = pos;
	debug.pos = box.pos + 1;
}

void Collision::setCenter(vec2 c) {
	box.pos = c - box.size / 2.f;
	debug.pos = box.pos + 1.f;
}

void Collision::moveCollision(vec2 mov) {
	box.pos += mov;
	debug.pos = box.pos + 1.f;
}

void Collision::drawDebug() {
	DrawRectangleLinesEx(debug, 1.f, color);
}
