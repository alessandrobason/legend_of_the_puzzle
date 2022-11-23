#pragma once
#include <iostream>
#include "Framework/Room.h"
#include "Framework/TextBox.h"
#include "Interactables/Statue.h"
#include "Interactables/ManInteractable.h"
#include "Player.h"
#include "GameObjects/Octorock.h"
#include "GameObjects/ManObject.h"

class Map_room : public Room {
public:
	Map_room(RoomManager* rm, sf::RenderWindow* window, InputHandler* input, std::string fold);
	~Map_room();

	void updateCamera();

	void load(vec2 offset) override;
	void setPlayer(Player* pl);
	void removePlayer();
	void setPlayerPosition(vec2 pos);
	vec2 getPlayerPosition() { return vec2(p->collider.rect.left, p->collider.rect.top); };
	Player* getPlayer() { return p; }
	rectf getBound(int i) { return bounds[i]; }
	vec2 getCameraTopLeft() { return camera_top_left; }
	void setCameraPosition(vec2 position) {
		main_camera.setCenter(position);
		camera_top_left = main_camera.getCenter() - main_camera.getSize() / 2.f;
		w->setView(main_camera);
	}

	void sortGameObjects();

	std::vector<GameObject*>* getGameObjects() { return &sceneObjects; }
	std::vector<Collision*>* getColliders() { return &sceneColliders; }

	void removeObject(GameObject* g);
	void freeMemory();

	void handleInput(float dt) override;
	void update(float dt) override;
	void draw() override;

	void start() override;

	void loadData();
	void resetRoom() override;

	void setPause(bool p) { paused = p; }
	bool getPause() { return paused; }

	Tilemap* getTilemap() { return tilemap; }

	vec2 moveRoom(sf::Transform t);
	void setBounds(vec2 offset);
	vec2 getOffset() { return offset; }
	
private:
	rectf bounds[4];

	vec2 camera_top_left;
	vec2 camera_bottom_right;

	bool paused = false;

	TextBox* textbox = nullptr;

	// tilemap data
	Tilemap* tilemap = nullptr;
	JSONparser* tilemap_json = nullptr;
	vec2 offset;
	std::vector<int>* layers = nullptr;
	Tilemap::tilemap_data* tilemap_data = nullptr;
	sf::Shader* shader = nullptr;

	std::vector<Interactable*> interactables;

	// enemies data, used to reset the scene
	std::vector<JSONparser::datatypes> enemiesData;

	// player pointer
	Player* p = nullptr;
};
