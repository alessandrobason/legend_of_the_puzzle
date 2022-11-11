#pragma once
// #include <SFML/Graphics.hpp>
#include <iostream>
#include "InputHandler.h"
#include "Collision.h"
#include "UsefulFunctions.h"
#include "RoomManager.fwd.h"
#include "FiniteStateMachine.h"
#include "sprite.h"

class GameObject : public FiniteStateMachine {
public:
	GameObject() {}
	GameObject(RoomManager* rm) {
		collider.setDebugColor(GREEN);
		roommanager = rm;
	}

	// Virtual update/draw function.
	virtual void handleInput(float dt) {}
	virtual void update(float dt) {}
	virtual void draw() {}
	virtual void drawDebug() { collider.drawDebug(); }

	virtual bool animationCallback(int anim_id) { std::cout << "gameobject: " << anim_id << "\n"; return false; }

	virtual void updateAnimation(float dt) {}

	virtual void isPlayerInside(bool p) {}

	virtual Sprite* getSprite() { return nullptr; }

	virtual void move(vec2 mov) {
		if (Sprite *spr = getSprite()) spr->pos += mov;
		collider.moveCollision(mov);
	}

	void flash(float dt) {
		flashing.timepassed += dt;
		if (flashing.timepassed >= 0.05f) {
			flashing.timepassed = 0.f;
			flashing.done++;
			if (flashing.done >= flashing.todo) {
				flashing.done = 0;
				flashing.timepassed = 0.f;
				flashing.isvisible = true;
				flashing.isflashing = false;
			}
			setVisible(flashing.isvisible);
			flashing.isvisible = !flashing.isvisible;
		}
	}

	virtual void setVisible(bool v) {
		if (Sprite *spr = getSprite()) 
			spr->color = flashing.color[v];
	}

	// Set the input component
	void setRoomManager(RoomManager* rm) { roommanager = rm; }

	float getY() { return collider.box.y; }

	virtual void setPosition(vec2 position) {
		collider.box.pos = position;
		collider.setDebugPosition(position + 1);
		if (Sprite *spr = getSprite()) spr->pos = position;
	}
	virtual vec2 getPosition() { if (Sprite *spr = getSprite()) return spr->pos; return {}; }

	vec2 getCenter() { return collider.box.pos + collider.box.size / 2.f; }

	bool moveTo(vec2 target, float dt) {
		vel = target - getCenter();
		if (vel.mag2() <= 1.f) return true;
		vel.normalise();
		move(vel * speed * dt);
		updateAnimation(dt);
		return false;
	}

	// Control sprite speed
	void setSpeed(float spd) { speed = spd; }
	float getSpeed() { return speed; }

	// Setter/getter for texture component
	void setTexture(Texture t) { txt = t; }
	Texture getTexture() { return txt; }

	virtual void hit(float damage) { std::cout << "Object hit\n"; }

	Collision collider;
	vec2 vel;
	vec2 oldVel;
	bool collided = false;

	Collision checkbox;

	Collision::LAYER collisionlayer = Collision::LAYER::ALL;

protected:
	// Sprite properties
	float speed = 0;

	bool dead = false;

	// pointer to the room manager
	RoomManager* roommanager = nullptr;

	Texture txt;

	struct FLASHING {
		bool isflashing = false;
		bool isvisible = true;
		Color color[2] = { BLANK, WHITE };
		float timepassed = 0.f;
		float flashtime = 0.6f;
		int todo = 6;
		int done = 0;
		float totaltime = flashtime;
	} flashing;

	vec2 checkboxsize = vec2(0, 0);
};