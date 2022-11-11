#pragma once
#include <string>
#include "../JSONparser/JSONparser.h"
#include "InputHandler.h"
#include "GameObject.h"
#include "Tilemap.h"
#include "Room.fwd.h"
#include "RoomManager.fwd.h"
#include "../camera.h"

class Room {
public:
	Room() {}
	Room(std::string fold, RoomManager* rm);
	~Room();

	virtual void handleInput(float dt) {}
	virtual void update(float dt) {}
	virtual void draw() {}
	virtual void load(vec2 offset) {}
	virtual void start() {}

	void setMainCamera(View c) { main_camera = c; }
	View getMainCamera() { return main_camera; }
	std::string getFile() { return FILE; }

	virtual void resetRoom() {}

	bool* isloaded = nullptr;

	struct gameobjectsorting {
		inline bool operator() (GameObject* g1, GameObject* g2) {
			return g1->getY() < g2->getY();
		}
	};

protected:
	// room manager
	RoomManager* roommanager = nullptr;

	// useful information
	std::string FILE;
	const std::string ASSETS = "Assets/";

	// main camera
	View main_camera;

	// vectors with scene objects/colliders
	std::vector<GameObject*> sceneObjects;
	std::vector<GameObject*> destroyedSceneObjects;
	std::vector<GameObject*> invisibleSceneObjects;
	std::vector<Collision*> sceneColliders;
	std::vector<Collision*> invisibleSceneColliders;
};
