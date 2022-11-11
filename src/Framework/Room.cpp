#include "Room.h"
#include "RoomManager.h"

Room::Room(std::string fold, RoomManager* rm){
	FILE = fold;
	std::cout << FILE + ".json" << "\n";

	roommanager = rm;
	main_camera = g_view;

	isloaded = new bool(false);
	// auto it = roommanager->tilemapData.find(fold);
	// if (it != roommanager->tilemapData.end()) {
	// 	isloaded = &it->second.loaded;
	// }
}

Room::~Room() {}