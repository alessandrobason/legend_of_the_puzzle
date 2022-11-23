#pragma once
#include <SFML/Graphics.hpp>

#include "Collision.h"
#include "RoomManager.fwd.h"

class Interactable : public Collision {
protected:
	RoomManager* roommanager = nullptr;

public:
	Interactable() {}
	Interactable(RoomManager* rm, rectf c);
	Interactable(RoomManager* rm, float x, float y, float w, float h);

	virtual void interact();
	virtual void reset() {}
};

