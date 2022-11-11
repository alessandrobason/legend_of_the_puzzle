#pragma once

#include <raylib.h>
#include <unordered_map>
#include <thread>
// #define WIN32_LEAN_AND_MEAN
// #include <Windows.h>
#include "Tweening.h"
#include "../JSONparser/JSONparser.h"
#include "InputHandler.h"
#include "Tilemap.h"
#include "sprite.h"
#include "RoomManager.fwd.h"
#include "../Map_room.h"
#if 0
#include "EventManager.h"
#include "Leaderboard.h"
#include "../PlayerGUI.h"
#include "../Menus/MainScreen.h"
#include "../Menus/SplashScreens.h"
#include "../Menus/MainMenu.h"
#include "../Menus/Option.h"
#include "../Menus/Ending.h"
#include "../Menus/PauseMenu.h"
#include "../Menus/ControlsMenu.h"
#include "../Menus/DeathScreen.h"
#include "../Menus/DebugMenu.h"
#include "../Menus/LeaderboardMenu.h"
#include "../GameObjects/ManObject.h"
#endif

#include "../maths.h"

class RoomManager {
public:
	enum RoomSides {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT
	};
	enum class STATES {
		MENU,
		MAP,
		MAPTRANSITION,
		MENUTRANSITION,
		GAMEOVER
	};

	bool first_time = true;

private:
	struct Map {
		int width = 0;
		int height = 0;
		int currentRoom = -1;
		int spawnpoint = -1;
		vec2 startposition;
		std::vector<int> data;
		std::vector<std::string> files;
	} map;

	struct MapMovement {
		int oldroom;
		Camera2D maincamera;
		// sf::View maincamera;
		vec2 offsetdirection;
		Tweening<vec2> playertween;
		Tweening<vec2> cameratween;
	} mapmovement;

	struct MenuTransition {
		Tweening<vec2> uppersquaretween;
		Tweening<vec2> bottomsquaretween;
		Tweening<float> musicvolumetween;
		rect uppersquare;
		rect bottomsquare;
		std::string newmenu;
		float oldvolume;
		float speed = 0.5f;
		bool actually_finished = false;
	} menutransitiondata;

	struct MusicLoading {
		std::string name;
		std::string path;
	};

	// dictionary with all the music of the game
	std::unordered_map<std::string, Music> musics;
	std::string currentsong = "";
	// dictionary with all the sounds of the game
	std::unordered_map<std::string, Sound> sounds;
	std::vector<std::string> hit_sounds;

	STATES currentstate;

	// std::unordered_map<std::string, Enemy> enemycopies;

	Player p;
	// PlayerGUI playergui;
	// ManObject man;
	int puzzlepiecesfound = 0;
	const int numberofpuzzlepieces = 7;
	// TextBox textbox;
	std::vector<Map_room*> maprooms;
	std::string currentroom = "";
	
	std::unordered_map<std::string, Room*> menuscreens;
	std::string currentmenu = "";
	std::string lastmenu = "";
	bool debug = false;

	JSONparser enemydata;
	Sprite cursor;

	std::string username = "";

	void loadMaps();
	void loadTextures();

	std::thread *loadingThread = nullptr;
public:

	RoomManager();
	~RoomManager();

	void init();

	void handleInput(float dt);
	void update(float dt);
	void draw();

	std::string getUsername() { return username; }
	
	Map_room* getCurrentRoom() { return maprooms[map.data[map.currentRoom]]; }
	JSONparser* getEnemyData() { return &enemydata; }
	// Enemy& getEnemyCopy(std::string enemy) { return enemycopies[enemy]; }
	
	void moveRoom(int side);
	void moveMenu(std::string newmenu);

	void restartGame();

	void setMenuTransitionOffset(std::string menu);

	std::string getLastMenu() { return lastmenu; }

	void loadMusic(std::vector<MusicLoading> musictoload);
	void playSong(std::string s);

	Music* getSong(std::string name) { return &musics[name]; }
	Music* getCurrentSong() { return  &musics[currentsong]; }
	std::unordered_map<std::string, Music>* getMusicMap() { return &musics; }

	void setVolume(float v);

	void loadSounds(std::vector<MusicLoading> soundstoload);
	void playSound(std::string s);
	void playHitSound();
	Sound* getCurrentSound(std::string s) { return &sounds[s]; }
	// sf::Sound* getSFMLSound(std::string s) { return &sounds[s].sound; }

	void animatetransition(float dt);
	void menuTransition(float dt);
	void drawMenuTransition();

	void loadEnemies();

	// TextBox* getTextBoxPointer() { return &textbox; }

	void setCurrentState(STATES s) { currentstate = s; }
	STATES getCurrentState() { return currentstate; }
	void setCurrentMenu(std::string m) { currentmenu = m; }
	std::string getCurrentMenu() { return currentmenu; }
	// void startCurrentMenu() { menuscreens[currentmenu]->start(); }

	Player* getPlayer() { return &p; }
	// PlayerGUI* getPlayerGui() { return &playergui; }
	// ManObject* getMan() { return &man; }

	bool isDebug() { return debug; }
	bool* getDebug() { return &debug; }
	void setDebug(bool d) { debug = d; }

	void findPiece() { puzzlepiecesfound++; }
	bool foundAllPieces() { return puzzlepiecesfound >= numberofpuzzlepieces; }
	int getNumberOfPiecesFound() { return puzzlepiecesfound; }

	vec2 getMousePos() const { return cursor.pos; }
	Texture loadTexture(const char *name, const char *filename);

	// dictionary with all the textures of the game
	// std::unordered_map<std::string, Texture> textures{};
	// std::unordered_map<std::string, Image> images{};
	// dictionary with all the fonts of the game
	// std::unordered_map<std::string, Font> fonts{};
	// dictionary with all the maps currently loaded
	// std::unordered_map<std::string, MapData> tilemapData{};

	// EventManager eventmanager;
	// Leaderboard httpleaderboard;
	float scoreclock = 0.f;

	bool ismusicplaying = true;
	float musicvolume = 0.f;

	int MAPSIZE = 16 * 32;
};
