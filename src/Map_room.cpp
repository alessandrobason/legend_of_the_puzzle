#include "Map_room.h"

#include <algorithm>

#include "Framework/RoomManager.h"

Map_room::Map_room(RoomManager* rm, std::string fold) : Room(fold, rm) {
	//auto it = roommanager->tilemapData.find(fold);
	//if (it != roommanager->tilemapData.end()) {
	//	layers = &it->second.layers;
	//	tilemap_data = &it->second.tilemap_data;
	//	tilemap = &it->second.tilemap;
	//}
	//else {
	//	auto res = roommanager->tilemapData.emplace(fold, MapData());
	//	if (res.second) {
	//		MapData &data = res.first->second;
	//		layers = &data.layers;
	//		tilemap_data = &data.tilemap_data;
	//		tilemap = &data.tilemap;
	//	}
	//}
	// shader = &roommanager->tilemapData[fold].shader;
	// textbox = roommanager->getTextBoxPointer();
}

Map_room::~Map_room() {
	//delete layers;
	//delete tilemap_data;
	//delete tilemap;
	// delete shader;
	//layers = nullptr;
	//tilemap_data = nullptr;
	//tilemap = nullptr;
	// shader = nullptr;
}

void Map_room::load(vec2 off) {
	if (*isloaded) return;
	offset = off;
	std::cout << "Loading: " << FILE << "\n";
	
	std::string tileset_json;
	std::string tileset_file;

	const std::string ASSETS = "Assets/";

	std::cout << "Loading json file\n";
	// load the json file for the map
	tilemap_json = JSONparser("Rooms/" + FILE + ".json");

	// load tileset json
	tileset_json = tilemap_json.doc["tilesets"].arr[0].obj["source"].str;
	//////////////////////
	size_t firstalph = tileset_json.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPWRSTUVWXYZ");
	if (firstalph == std::string::npos) {
		std::cout << "There's been a problem with the tileset filename: " << tileset_json << "\n";
		abort();
	}
	tileset_json.replace(0, firstalph, "");
	if (tileset_json[0] != 'A') {
		tileset_json = "Assets/" + tileset_json;
	}
	
	JSONparser* ts_json = new JSONparser(tileset_json);

	std::vector<Tilemap::animated_tiles_data> animatedTilesData;

	// load tilemap data
	tilemap_data.numb_layers = (int)tilemap_json.doc["layers"].arr.size();
	tilemap_data.tileSize.x = tilemap_json.doc["tilewidth"].i;
	tilemap_data.tileSize.y = tilemap_json.doc["tileheight"].i;
	tilemap_data.h = tilemap_json.doc["height"].i;
	tilemap_data.w = tilemap_json.doc["width"].i;

	std::cout << "Map json file: " << FILE << ".json\n";
	std::cout << "Map height: " << tilemap_data.h << "\n";
	std::cout << "Map width: " << tilemap_data.w << "\n";
	std::cout << "Number of layers: " << tilemap_data.numb_layers << "\n";

	int numb_layers = tilemap_data.numb_layers;
	std::vector<Collision> collisions;

	// LOAD LAYERS DATA //
	for (int i = 0; i < numb_layers; i++) {
		std::cout << "Layer " << i + 1 << ": \n";
		std::cout << "\tName: " << tilemap_json.doc["layers"].arr[i].obj["name"].str << "\n";

		// load collisions
		if (tilemap_json.doc["layers"].arr[i].obj["type"].str == "objectgroup") {
			tilemap_data.numb_layers--;
			if (tilemap_json.doc["layers"].arr[i].obj["name"].str == "collisions") {
				for (size_t k = 0; k < tilemap_json.doc["layers"].arr[i].obj["objects"].arr.size(); k++) {
					JSONparser::datatypes* current = &tilemap_json.doc["layers"].arr[i].obj["objects"].arr[k];
					float x = offset.x + (float)current->obj["x"].i;
					float y = offset.y + (float)current->obj["y"].i;
					float w = (float)current->obj["width"].i;
					float h = (float)current->obj["height"].i;
					Collision::LAYER layer = Collision::LAYER::ALL;
					if (current->obj["type"].str == "water") {
						layer = Collision::LAYER::WATER;
					}
					else if (current->obj["type"].str == "interactable") {
						if (current->obj["name"].str == "puzzle_piece") {
							// interactables.push_back(new Statue(roommanager, x, y, width, height));
						}
						else if (current->obj["name"].str == "man") {
							// interactables.push_back(new ManInteractable(roommanager, x, y, width, height));
						}
					}
					collisions.push_back(Collision(x, y, w, h, layer));
				}
			}
			else if (tilemap_json.doc["layers"].arr[i].obj["name"].str == "spawnpoints") {
				enemiesData = tilemap_json.doc["layers"].arr[i].obj["objects"].arr;
			}
		}
		else {

			// load tileset
			for (int j = 0; j < tilemap_json.doc["layers"].arr[i].obj["data"].arr.size(); j++) {
				int t = tilemap_json.doc["layers"].arr[i].obj["data"].arr[j].i;
				t--; // tiled starts at 1 instead of 0
				layers.push_back(t);
			}
		}
	}

	// load tileset data

	tileset_file = ts_json->doc["image"].str;

	std::cout << "Tileset json file: " << tileset_json << "\n";
	std::cout << "Tileset image file: " << tileset_file << "\n";
	Image tiles = LoadImage(tileset_file.c_str());
	// if (!tiles.data) {
	// 	std::cout << "Couldn't load tileset from " << tileset_file << "\n";
	// 	abort();
	// }
	// roommanager->images["tiles"] = tiles;

	std::cout << "number of tiles: " << ts_json->doc["tiles"].size << "\n";

	// loading animated tiles
	std::unordered_map<std::string, JSONparser::datatypes>::const_iterator it;
	for (auto const& x : ts_json->doc["tiles"].obj) {
		it = x.second.obj.find("animation");
		if (it != x.second.obj.end()) {
			std::vector<Tilemap::animated_tiles_data::data> temp;
			for (size_t i = 0; i < it->second.arr.size(); i++) {
				temp.push_back({ it->second.arr[i].obj.at("duration").i , it->second.arr[i].obj.at("tileid").i });
			}
			animatedTilesData.push_back({ std::stoi(x.first), std::move(temp) });
		}
	}

	std::cout << "animatedTilesData: " << animatedTilesData.size() << "\n";

	tilemap.load(roommanager->getDebug(), tiles, animatedTilesData, layers, tilemap_data, offset);
	tilemap.loadCollisions(std::move(collisions));

	loadData();

	delete ts_json;
	ts_json = nullptr;

	*isloaded = true;

	setBounds(offset);
}

void Map_room::setPlayer(Player* pl) {
	p = pl;
	sceneObjects.push_back(p);
}

void Map_room::removePlayer() {
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		if (sceneObjects[i] == p){
			sceneObjects[i] = sceneObjects.back();
			sceneObjects.pop_back();
			// sceneObjects.erase(sceneObjects.begin() + i);
		}
	}
	p = nullptr; 
}

void Map_room::setPlayerPosition(vec2 pos) {
	p->setPosition(pos);
}

void Map_room::loadData() {
	for (size_t i = 0; i < tilemap.collisions.size(); i++) {
		sceneColliders.push_back(&tilemap.collisions[i]);
	}

	std::cout << "\t##### LOADING ENEMIES #####\n";
	for (size_t k = 0; k < enemiesData.size(); k++) {
		JSONparser::datatypes* current = &enemiesData[k];
		std::string name = current->obj["template"].str;
		vec2 position = offset;
		position.x += current->obj["x"].i;
		position.y += current->obj["y"].i - 16; // tiled starts at 16 for some reason

		size_t firstalph = name.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPWRSTUVWXYZ");

		// removes all character before the enemy name
		name.replace(0, firstalph, "");
		// removes all character after the enemy name
		name.replace(name.end() - 3, name.end(), "");

		if (name == "octorock") {
			// sceneObjects.push_back(new Octorock(roommanager->getEnemyCopy(name)));
		}

		// sceneObjects.back()->setPosition(position);
		// sceneColliders.push_back(&sceneObjects.back()->collider);
		std::cout << "\t\tname: " << name << "\n";
		std::cout << "\t\tposition: " << position.x << " " << position.y << "\n";
	}

	if (FILE == "plateau") {
		// sceneObjects.push_back(roommanager->getMan());
	}
}

void Map_room::resetRoom() {
	GameObject* player = nullptr;
	for (auto &obj : sceneObjects) {
		if (obj == p) continue;
		// if (obj == roommanager->getMan()) continue;
		delete obj;
		obj = nullptr;
	}
	p = nullptr;

	sceneObjects.clear();
	sceneColliders.clear();

	// for (auto &inter : interactables) {
	// 	inter->reset();
	// }

	loadData();

	paused = false;
}

/*######################*\
||                      ||
||     HANDLE INPUT     ||
||                      ||
\*######################*/
void Map_room::handleInput(float dt) {
	if (paused) {
		return;
	}

	if (IsKeyPressed(KeyDebug)) {
		roommanager->moveMenu("debugmenu");
	}

	for (auto &obj : sceneObjects) {
		obj->handleInput(dt);
	}

	if (IsKeyPressed(KeyInteract)) {
		rect check_interact = p->collider.box;

		switch ((int)(p->getAngleToMouse() / 45)) {
		case 1:
		case 2:
			check_interact.y -= 1;
			break;
		case 3:
		case 4:
			check_interact.x += 1;
			break;
		case 5:
		case 6:
			check_interact.y += 1;
			break;
		case 7:
		case 0:
			check_interact.x -= 1;
			break;
		}

		// for (auto &inter : interactables) {
		// 	if (inter->Check_Collision(check_interact)) {
		// 		inter->interact();
		// 	}
		// }
	}
}

/*######################*\
||                      ||
||        UPDATE        ||
||                      ||
\*######################*/
void Map_room::update(float dt) {
	tilemap.animate(dt);

	if (paused) {
		// textbox->update(dt);
		// roommanager->eventmanager.update(dt);
		return;
	}

	roommanager->scoreclock += dt;
	//update every gameobject (player too)
	for (auto &obj : sceneObjects) {
		if (obj != p) {
			obj->Advance();
			//check if player has gone inside gameobject checkbox (used for enemies)
			rect box = obj->checkbox.box;
			obj->isPlayerInside(p->checkbox.Check_Collision(box));
		}
		obj->update(dt);
	}

	// check object collisions against tilemap
	for (auto &coll : sceneColliders) {
		coll->setDebugColor(RED);
		for (auto &obj : sceneObjects) {
			if (obj == p && p->getCheat(Player::CHEATS::WALKTHROUGHWALLS)) {
				continue;
			}
			if (obj->collider.Check_Collision(coll->box)) {
				vec2 revVel = obj->collider.getCollisionSide(coll->box, obj->oldVel);
				obj->move(revVel);
				obj->collided = true;
				coll->setDebugColor(BLUE);
			}
		}
	}

	// check if player is going in another map //
	for (int i = 0; i < 4; i++) {
		if (p->collider.Check_Collision(bounds[i])) {
			roommanager->moveRoom(i);
			return;
		}
	}

	updateCamera();

	// roommanager->eventmanager.update(dt);
}

void Map_room::updateCamera() {
	main_camera = g_view;
	main_camera.target = p->getSprite()->pos + p->getLocalCenter();
	camera_top_left = GetScreenToWorld2D({ 0, 0 }, main_camera);

	vec2 offset = main_camera.target - camera_top_left;
	camera_bottom_right = camera_top_left + resolution;
	// camera_top_left = main_camera.getCenter() - main_camera.getSize() / 2.f;
	// camera_bottom_right = main_camera.getCenter() + main_camera.getSize() / 2.f;

	vec2 res = (vec2)resolution;

	// -- STICK CAMERA --
	// left
	if (camera_top_left.x <= bounds[2].x + bounds[2].w) {
		main_camera.target.x = bounds[2].x + res.x / 2.f;
		// main_camera.setCenter(vec2(bounds[2].left + (main_camera.getSize().x / 2.f), main_camera.getCenter().y));
	}
	// right
	if (camera_bottom_right.x >= bounds[3].x) {
		main_camera.target.x = bounds[3].x + res.x / 2.f;
		// main_camera.setCenter(vec2(bounds[3].left - (main_camera.getSize().x / 2.f), main_camera.getCenter().y));
	}
	// top
	if (camera_top_left.y <= bounds[0].y) {
		main_camera.target.y = bounds[0].y + res.y / 2.f;
		// main_camera.setCenter(vec2(main_camera.getCenter().x, bounds[0].top + main_camera.getSize().y / 2.f));
	}
	// bottom
	if (camera_bottom_right.y >= bounds[1].y + bounds[1].h) {
		main_camera.target.y = bounds[1].y + res.y / 2.f;
		// main_camera.setCenter(vec2(main_camera.getCenter().x, bounds[1].top - (main_camera.getSize().y / 2.f)));
	}
	// update top_left of the camera for player
	camera_top_left = GetScreenToWorld2D({ 0, 0 }, main_camera);
	g_view = main_camera;
	// camera_top_left = main_camera.getCenter() - main_camera.getSize() / 2.f;
	// w->setView(main_camera);
}

/*######################*\
||                      ||
||         DRAW         ||
||                      ||
\*######################*/
void Map_room::draw() {
	sortGameObjects();
	tilemap.draw();
	for (auto &obj : sceneObjects) {
		obj->draw();
	}

	// draw debug
	if (roommanager->isDebug()) {
		if(p != nullptr) p->drawDebug();
		tilemap.drawDebug();
		for (auto &obj : sceneObjects) {
			obj->drawDebug();
		}
		// for (auto &inter : interactables) {
		// 	inter->drawDebug();
		// }
	}

	// roommanager->eventmanager.draw();
}

void Map_room::start() {
#if 0
	roommanager->playSong("game");
	if (roommanager->eventmanager.getNumberOfEvents() == 0) setPause(false);
#endif
}

void Map_room::removeObject(GameObject* g) {
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		if (sceneObjects[i] == g) {
			//destroyedSceneObjects.push_back(sceneObjects[i]);
			sceneObjects[i] = sceneObjects.back();
			sceneObjects.pop_back();
			break;
		}
	}
	for (size_t i = 0; i < sceneColliders.size(); i++) {
		if (sceneColliders[i] == &g->collider) {
			sceneColliders[i] = sceneColliders.back();
			sceneColliders.pop_back();
			break;
		}
	}
}

void Map_room::freeMemory() {
	for (auto &obj : destroyedSceneObjects) {
		delete obj;
	}
	destroyedSceneObjects.clear();
}

void Map_room::sortGameObjects() {
	std::sort(sceneObjects.begin(), sceneObjects.end(), gameobjectsorting());
}

#if 0
vec2 Map_room::moveRoom(sf::Transform t) {
	sf::RenderStates tilemapstate = tilemap.getStates();
	tilemapstate.transform = t;
	tilemap.setStates(tilemapstate);
	return tilemap.getPosition();
}
#endif

void Map_room::setBounds(vec2 offset) {
	this->offset = offset;
	bounds[0] = rect(offset.x,						  offset.y, 					   (float)roommanager->MAPSIZE, 0.1f); //top
	bounds[1] = rect(offset.x,						  offset.y + roommanager->MAPSIZE, (float)roommanager->MAPSIZE, 0.1f); //bottom
	bounds[2] = rect(offset.x,						  offset.y, 					   0.1f	, 					    (float)roommanager->MAPSIZE); //left
	bounds[3] = rect(offset.x + roommanager->MAPSIZE, offset.y, 					   0.1f	, 					    (float)roommanager->MAPSIZE); //right
}