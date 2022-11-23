#include "RoomManager.h"

#include "Tilemap.h"
#include "../Menus/MainScreen.h"
#include "../Menus/SplashScreens.h"
#include "../Menus/MainMenu.h"
#include "../Menus/Option.h"
#include "../Menus/Ending.h"
#include "../Menus/PauseMenu.h"
#include "../Menus/ControlsMenu.h"
#include "../Menus/DeathScreen.h"
#include "../Menus/DebugMenu.h"

RoomManager::RoomManager() {
	currentstate = STATES::MENU;
	currentmenu = "splashscreen";
	JSONparser soundsjson = JSONparser("Data/sounds_to_load.json");

	std::vector<MusicLoading> musictoload;
	std::vector<MusicLoading> soundstoload;
	int number_of_hit_sounds = 0;

	// load music
	for (size_t i = 0; i < soundsjson.doc["music"].arr.size(); i++) 
		musictoload.push_back({ soundsjson.doc["music"].arr[i].obj["name"].str, soundsjson.doc["music"].arr[i].obj["file"].str });

	// load sounds
	for (size_t i = 0; i < soundsjson.doc["sounds"].arr.size(); i++) {
		std::string filename = soundsjson.doc["sounds"].arr[i].obj["file"].str;
		std::string name = soundsjson.doc["sounds"].arr[i].obj["name"].str;
		if (name == "hit") {
			name += "_" + std::to_string(number_of_hit_sounds);
			hit_sounds.push_back(name);
			number_of_hit_sounds++;
		}
		soundstoload.push_back({ name, filename });
	}

	loadMusic(musictoload);
	loadSounds(soundstoload);

	eventmanager = EventManager(this);

	// get username from computer
	// will not work if the name is longer than 255 characters
	// really hope nobody has that long of a name
	// from https://docs.microsoft.com/it-it/windows/win32/api/winbase/nf-winbase-getusernamea
	char usernamebuffer[255];
	unsigned long buffersize = sizeof(usernamebuffer);
	GetUserNameA(usernamebuffer, &buffersize);
	username = usernamebuffer;
}

void RoomManager::setData(sf::RenderWindow* win, InputHandler* inp) {
	w = win; 
	in = inp;

	textbox = TextBox(this, in);

	// set mouse cursor
	if (!textures["cursor"].loadFromFile("Assets/cursor.png")) {
		std::cout << "failed to load mouse texture\n";
		abort();
	}
	cursor.setTexture(textures["cursor"]);
	cursor.setOrigin(vec2(3.5f, 3.5f));

	w->setMouseCursorVisible(false);
	
	menutransitiondata.uppersquaretween = Tweening<vec2>(vec2(in->getScreenSize().x, 0), vec2(in->getScreenSize().x, in->getScreenSize().y / 2), 0.5f);
	menutransitiondata.bottomsquaretween = Tweening<vec2>(vec2(in->getScreenSize().x, 0), vec2(in->getScreenSize().x, -in->getScreenSize().y / 2), 0.5f);

	menutransitiondata.uppersquare = sf::RectangleShape(vec2(in->getScreenSize().x, 0));
	menutransitiondata.uppersquare.setPosition(vec2());
	menutransitiondata.uppersquare.setFillColor(sf::Color::Black);
	menutransitiondata.bottomsquare = sf::RectangleShape(vec2(in->getScreenSize().x, 0));
	menutransitiondata.bottomsquare.setPosition(vec2(0, in->getScreenSize().y));
	menutransitiondata.bottomsquare.setFillColor(sf::Color::Black);

	menuscreens["splashscreen"] = new SplashScreens(w, in, this);
	menuscreens["startscreen"] = new MainScreen(w, in, this);
	menuscreens["mainmenu"] = new MainMenu(w, in, this);
	menuscreens["options"] = new OptionMenu(w, in, this);
	menuscreens["pause"] = new PauseMenu(w, in, this);
	menuscreens["controls"] = new ControlsMenu(w, in, this);
	menuscreens["deathscreen"] = new DeathScreen(w, in, this);
	menuscreens["debugmenu"] = new DebugMenu(w, in, this);
	menuscreens["ending"] = new Ending(w, in, this);
	// menuscreens["leaderboard"] = new LeaderboardMenu(w, in, this);

	p = Player(in, this, w);
	p.getAnimation()->setCallbackObject(&p);

	enemydata = JSONparser("Data/enemy_data.json");

	JSONparser worldmap = JSONparser("Rooms/worldmap.json");
	map.width = worldmap.doc["width"].i;
	map.height = worldmap.doc["height"].i;

	for (size_t i = 0; i < worldmap.doc["Levels"].arr.size(); i++) {
		map.files.push_back(worldmap.doc["Levels"].arr[i].str);
		maprooms.push_back(new Map_room(this, w, in, map.files[i]));
	}
	for (size_t i = 0; i < worldmap.doc["map"].arr.size(); i++) {
		map.data.push_back(worldmap.doc["map"].arr[i].i);
	}

	map.spawnpoint = worldmap.doc["spawn"].obj["map"].i;
	map.currentRoom = map.spawnpoint;
	maprooms[map.data[map.currentRoom]]->setPlayer(&p);

	loadEnemies();
	loadMaps();
	menuscreens[currentmenu]->start();

	map.startposition = vec2i(worldmap.doc["spawn"].obj["position"].arr[0].i, worldmap.doc["spawn"].obj["position"].arr[1].i);
	p.setPosition(maprooms[map.data[map.currentRoom]]->getOffset() + map.startposition);

	loadTextures();

	// starting cutscene

	man = ManObject(in, this, w);

	getCurrentRoom()->setPause(true);
	
	Event::EventInput dialog;
	Event::EventInput moveman;
	Event::EventInput cameramovement;
	/////////////////
	cameramovement.cameramovement.push_back({ getCurrentRoom()->getOffset() + vec2(248, 96), 0.f});
	cameramovement.cameramovement.push_back({vec2(0, 112), 3.f});
	eventmanager.addEvent(EventManager::EventType::MOVETO, cameramovement);
	/////////////////
	dialog.dialog = { "I need your\nhelp." };
	eventmanager.addEvent(EventManager::EventType::SAY, dialog);
	/////////////////
	moveman.character = &man;
	moveman.target = man.getCenter() + vec2(-32, 0);
	eventmanager.addEvent(EventManager::EventType::MOVETO, moveman);
	/////////////////
	dialog.dialog = { "They stole them." };
	eventmanager.addEvent(EventManager::EventType::SAY, dialog);
	/////////////////
	moveman.target = man.getCenter() + vec2(+32, 0);
	eventmanager.addEvent(EventManager::EventType::MOVETO, moveman);
	/////////////////
	dialog.dialog = { "They stole all of them." };
	eventmanager.addEvent(EventManager::EventType::SAY, dialog);
	/////////////////
	moveman.target = man.getCenter();
	eventmanager.addEvent(EventManager::EventType::MOVETO, moveman);
	/////////////////
	moveman.target = man.getCenter() + vec2(0, 16);
	eventmanager.addEvent(EventManager::EventType::MOVETO, moveman);
	/////////////////
	dialog.dialog = {
		"THEY STOLE ALL MY\nPUZZLE PIECES.",
		"It is truly unforgivable.",
		"So please, go find my \n7 puzzle pieces for me.",
		"They are scattered all\nthroughout the land.",
		"I managed to give them\nto 7 guardians,",
		"the one on the surface\nare golden statues.",
		"the ones underwater\nare trees.",
		"go now, bring me my\npuzzle pieces."
	};
	eventmanager.addEvent(EventManager::EventType::SAY, dialog);
	/////////////////
	moveman.target = man.getCenter();
	eventmanager.addEvent(EventManager::EventType::MOVETO, moveman);
	/////////////////
}

RoomManager::~RoomManager() {
	delete in;
	in = nullptr;
}

void RoomManager::loadEnemies() {
	if (!textures["enemy"].loadFromFile(enemydata.doc["spritesheet"].str)) {
		std::cout << "Couldn't load enemy spritesheet\n";
		abort();
	}
	std::cout << "Loaded enemy spritesheet\n";

	if (!textures["enemyproj"].loadFromFile("Assets/enemyprojectiles.png")) {
		std::cout << "couldn't load enemies projectiles\n";
		abort();
	}
	std::cout << "Loaded enemies projectiles texture\n";

	for (size_t i = 0; i < enemydata.doc["enemies"].arr.size(); i++) {
		JSONparser::datatypes* current = &enemydata.doc["enemies"].arr[i];
		Enemy e;
		e.setTexture(&textures["enemy"]);
		e.setInput(in);
		e.setWindow(w);
		e.setRoomManager(this);
		std::string name = current->obj["name"].str;
		float x = (float)current->obj["collision"].obj["x"].i;
		float y = (float)current->obj["collision"].obj["y"].i;
		float width  = (float)current->obj["collision"].obj["width"].i;
		float height = (float)current->obj["collision"].obj["height"].i;
		e.collisionlayer = Collision::LAYER::ENEMY;
		e.collider = Collision(x, y, width, height, e.collisionlayer);
		
		int rows = current->obj["rows"].i;
		int columns = current->obj["columns"].i;
		AnimatedSprite animatedsprite;
		animatedsprite.setSpriteSheet(&textures["enemy"], columns, rows);
    	auto &json_animations = current->obj["animations"].arr;
		for (auto &anim : json_animations) {
			std::vector<int> animFrames;
			float speed = (float)anim.obj["speed"].d;
			bool looping = anim.obj["looping"].b;
			for (auto &frame : anim.obj["array"].arr) {
				animFrames.emplace_back(frame.i);
			}
			auto &anim_name = anim.obj["name"].str;
			animatedsprite.addAnimation(anim_name, std::move(animFrames), speed, looping);
		}
		animatedsprite.setCurrentAnimation(current->obj["animations"].arr[0].obj["name"].str);
		
		e.setAnimatedSprite(animatedsprite);
		e.setProjectile(current->obj["projectile"].b);
		e.setSpeed((float)current->obj["speed"].i);
		e.setLife((float)current->obj["life"].d);
		e.setDamage((float)current->obj["damage"].d);

		enemycopies[name] = e;
	}
}

void RoomManager::handleInput(float dt) {
	switch (currentstate) {
	case RoomManager::STATES::MENU:
		menuscreens[currentmenu]->handleInput(dt);
		break;
	case RoomManager::STATES::MAP:
		maprooms[map.data[map.currentRoom]]->handleInput(dt);
		break;
	}
}

void RoomManager::update(float dt) {
	cursor.setPosition(in->getMouseRelative());
	switch (currentstate) {
	case RoomManager::STATES::MENU:
		menuscreens[currentmenu]->update(dt);
		break;
	case RoomManager::STATES::MAP:
		playergui.update(dt);
		maprooms[map.data[map.currentRoom]]->update(dt);
		break;
	case RoomManager::STATES::MAPTRANSITION:
		playergui.update(dt);
		animatetransition(dt);
		break;
	case RoomManager::STATES::MENUTRANSITION:
		if (currentmenu == "game") getCurrentRoom()->update(dt);
		menuTransition(dt);
		break;
	case RoomManager::STATES::GAMEOVER:
		p.getAnimation()->animate(dt);
		break;
	}
}

void RoomManager::draw() {
	w->clear();
	sf::View camera = w->getView();
	camera.setViewport(in->getView().getViewport());
	w->setView(camera);
	w->m_view.begin();
	switch (currentstate) {
	case RoomManager::STATES::MENU:
		menuscreens[currentmenu]->draw();
		break;
	case RoomManager::STATES::MAP:
		maprooms[map.data[map.currentRoom]]->draw();
		playergui.draw(w);
		break;
	case RoomManager::STATES::MAPTRANSITION:
		maprooms[mapmovement.oldroom]->draw();
		maprooms[map.data[map.currentRoom]]->draw();
		playergui.draw(w);
		break;
	case RoomManager::STATES::MENUTRANSITION:
		if (menutransitiondata.newmenu == "deathscreen" && currentmenu != "deathscreen") w->draw(*p.getSprite());
		else if (currentmenu == "game") maprooms[map.data[map.currentRoom]]->draw();
		else menuscreens[currentmenu]->draw();
		drawMenuTransition();
		break;
	case RoomManager::STATES::GAMEOVER:
		w->draw(*p.getSprite());
		break;
	}
	w->draw(cursor);
	w->m_view.end();
	w->display();
}

void RoomManager::loadMaps() {
	struct load {
		int position;
		int map;
	};
	std::vector<load> toLoad;
	toLoad.push_back({ map.currentRoom, map.data[map.currentRoom] });
	// on the left
	if (map.currentRoom % map.width > 0 && map.data[(int)map.currentRoom - 1] != -1)
		toLoad.push_back({ map.currentRoom - 1, map.data[(int)map.currentRoom - 1] });
	// on the right
	if (map.currentRoom % map.width < map.width-1 && map.data[(int)map.currentRoom + 1] != -1)
		toLoad.push_back({ map.currentRoom + 1, map.data[(int)map.currentRoom + 1] });
	// on top
	if (map.currentRoom >= map.width && map.data[(int)map.currentRoom - map.width] != -1)
		toLoad.push_back({ map.currentRoom - map.width, map.data[(int)map.currentRoom - map.width] });
	// on bottom
	if (map.currentRoom / map.width < map.height - 1 && map.data[(int)map.currentRoom + map.width] != -1)
		toLoad.push_back({ map.currentRoom + map.width, map.data[(int)map.currentRoom + map.width] });
	
	for (size_t i = 0; i < toLoad.size(); i++) {
		if (*maprooms[toLoad[i].map]->isloaded == true) continue;
		int mapx = toLoad[i].position % map.width;
		int mapy = toLoad[i].position / map.width;
		std::cout << "load: " << toLoad[i].map << "\n";
		vec2 offset = vec2i(mapx * MAPSIZE, mapy * MAPSIZE);
		
		maprooms[toLoad[i].map]->load(offset);
	}
}

void RoomManager::loadTextures() {
	for (std::pair<std::string, sf::Image> img : images) {
		if (!textures[img.first].loadFromImage(img.second)) {
			std::cout << "couldn't load " << img.first << "\n";
		}
	}
	for (size_t i = 0; i < maprooms.size(); i++) {
		if (!maprooms[i]->isloaded) continue;
		maprooms[i]->getTilemap()->setTexture(&textures["tiles"]);
	}
}

void RoomManager::moveRoom(int side) {
	int oldcurrentroom = map.data[map.currentRoom];
	sf::View maincamera = maprooms[oldcurrentroom]->getMainCamera();
	vec2 oldplayerpos = maprooms[oldcurrentroom]->getPlayerPosition();
	Player::DIRECTIONS lastplayerdirection = p.getDirection();
	vec2 offsetdirection;
	rectf bound;
	switch (side) {
	case TOP:
		if (map.currentRoom - map.width  < 0) return;
		map.currentRoom -= map.width;
		offsetdirection = vec2(0, -1);
		break;
	case BOTTOM:
		if (map.currentRoom + map.width >= map.data.size()) return;
		map.currentRoom += map.width;
		offsetdirection = vec2(0, +1);
		break;
	case LEFT:
		if (map.currentRoom%map.width == 0) return;
		map.currentRoom--;
		offsetdirection = vec2(-1, 0);
		break;
	case RIGHT:
		if ((map.currentRoom%map.width)+1 >= map.width) return;
		map.currentRoom++;
		offsetdirection = vec2(1, 0);
		break;
	}

	std::thread loadingThread(&RoomManager::loadMaps, this);
	loadingThread.detach();

	int currentroom = map.data[map.currentRoom];

	maprooms[oldcurrentroom]->removePlayer();
	maprooms[currentroom]->setPlayer(&p);
	
	mapmovement.oldroom = oldcurrentroom;
	mapmovement.maincamera = maincamera;
	mapmovement.offsetdirection = offsetdirection;

	// LERPING
	// -- camera
	vec2 finalcamera = maincamera.getCenter() + vec2(maincamera.getSize().x * offsetdirection.x, maincamera.getSize().y * offsetdirection.y);
	mapmovement.cameratween = Tweening<vec2>(maincamera.getCenter(), finalcamera, 0.5f);
	// -- player
	vec2 startPlayer = oldplayerpos - p.collider.collision_offset;
	vec2 finalplayer = startPlayer + (offsetdirection * 17.f);
	mapmovement.playertween = Tweening<vec2>(startPlayer, finalplayer, 0.3f);
	
	currentstate = STATES::MAPTRANSITION;
}

void RoomManager::loadMusic(std::vector<MusicLoading> musictoload) {
	for (size_t i = 0; i < musictoload.size(); i++) {
		if (!musics[musictoload[i].name].openFromFile("Assets/music/" + musictoload[i].path)) {
			std::cout << "couldn't open music file for " << musictoload[i].name << " from " << musictoload[i].path << "\n";
			// abort();
		}
		musics[musictoload[i].name].setLoop(true);
	}
}

void RoomManager::playSong(std::string s) {
	if (s == currentsong) return;
	//float oldvolume = musics[currentsong].getVolume();
	musics[currentsong].pause();
	currentsong = s;
	musics[currentsong].setVolume(musicvolume);
	if (ismusicplaying) musics[currentsong].play();
}

void RoomManager::loadSounds(std::vector<MusicLoading> soundstoload) {
	for (size_t i = 0; i < soundstoload.size(); i++) {
		MusicLoading* s = &soundstoload[i];
		if (sounds[s->name].buffer.loadFromFile("Assets/sounds/" + s->path)) {
			std::cout << "Loaded " << soundstoload[i].name << "\n";
			sounds[s->name].sound.setBuffer(sounds[s->name].buffer);
		}
		else {
			std::cout << "Couldn't load " << soundstoload[i].name << ", file: " << soundstoload[i].path << "\n";
			abort();
		}
	}
}

void RoomManager::playSound(std::string s) {
	if (ismusicplaying) {
		float oldvolume = musics[currentsong].getVolume();
		getSFMLSound(s)->setVolume(oldvolume);
		getSFMLSound(s)->play();
	}
}

void RoomManager::playHitSound() {
	int s = rand() % hit_sounds.size();
	playSound(hit_sounds[s]);
}

void RoomManager::setVolume(float v) {
	musicvolume = v;
	getCurrentSong()->setVolume(v);
	for (auto sound : sounds) {
		sound.second.sound.setVolume(v);
	}
}

void RoomManager::animatetransition(float dt) {
	mapmovement.playertween.update(dt);
	mapmovement.cameratween.update(dt);
	
	vec2 newplayerposition;
	if (!mapmovement.playertween.isfinished()) {
		newplayerposition = mapmovement.playertween.getValue();
		p.setPosition(newplayerposition);
		for (auto c : *getCurrentRoom()->getColliders()) {
			if (p.collider.Check_Collision(c->rect)) {
				vec2 revVel = p.collider.getCollisionSide(c->rect, p.oldVel);
				p.move(revVel);
				p.collided = true;
				c->setDebugColor(sf::Color::Blue);
			}
		}
	}

	vec2 newcameraposition;
	newcameraposition = mapmovement.cameratween.getValue();
	mapmovement.maincamera.setCenter(newcameraposition);
	w->setView(mapmovement.maincamera);
	if (mapmovement.cameratween.isfinished()) {
		getCurrentRoom()->setMainCamera(mapmovement.maincamera);
		currentstate = STATES::MAP;
	}
}

void RoomManager::moveMenu(std::string newmenu) {
	lastmenu = currentmenu;
	menutransitiondata.newmenu = newmenu;
	if (newmenu == "game") getCurrentRoom()->setPause(true);
	menutransitiondata.musicvolumetween = Tweening<float>(getCurrentSong()->getVolume(), 0.f, menutransitiondata.speed);
	menutransitiondata.oldvolume = getCurrentSong()->getVolume();
	currentstate = STATES::MENUTRANSITION;
	setMenuTransitionOffset(currentmenu);
}

void RoomManager::restartGame() {
	p.reset();
	playergui.reset();
	puzzlepiecesfound = 0;
	map.currentRoom = map.spawnpoint;
	for (size_t i = 0; i < maprooms.size(); i++) {
		if(maprooms[i]->isloaded) maprooms[i]->resetRoom();
	}
	p.setPosition(getCurrentRoom()->getOffset() + map.startposition);
	getCurrentRoom()->setPlayer(&p);
	menuscreens["ending"]->resetRoom();
}

void RoomManager::menuTransition(float dt) {
	menutransitiondata.uppersquaretween.update(dt);
	menutransitiondata.bottomsquaretween.update(dt);
	menutransitiondata.musicvolumetween.update(dt);

	sf::View camera = w->getView();
	vec2 offset = camera.getCenter() - camera.getSize() / 2.f;;

	menutransitiondata.uppersquare.setSize(menutransitiondata.uppersquaretween.getValue());
	menutransitiondata.uppersquare.setPosition(offset);
	menutransitiondata.bottomsquare.setSize(menutransitiondata.bottomsquaretween.getValue());
	menutransitiondata.bottomsquare.setPosition(offset+ vec2(0, in->getScreenSize().y));

	if (lastmenu == "game" || menutransitiondata.newmenu == "game" || lastmenu == "ending") {
		setVolume(menutransitiondata.musicvolumetween.getValue());
	}

	if (menutransitiondata.uppersquaretween.isfinished() && menutransitiondata.bottomsquaretween.isfinished()) {
		std::string newmenu = menutransitiondata.newmenu;

		if (menutransitiondata.actually_finished) {
			if (newmenu != "game") {
				currentmenu = newmenu;
				currentstate = STATES::MENU;
			}
			else {
				currentstate = STATES::MAP;
				getCurrentRoom()->start();
				p.setCanMove(true);
			}
			menutransitiondata.actually_finished = false;
			menutransitiondata.uppersquaretween = Tweening<vec2>(vec2(in->getScreenSize().x, 0), vec2(in->getScreenSize().x, in->getScreenSize().y / 2), menutransitiondata.speed);
			menutransitiondata.bottomsquaretween = Tweening<vec2>(vec2(in->getScreenSize().x, 0), vec2(in->getScreenSize().x, -in->getScreenSize().y / 2), menutransitiondata.speed);
			menutransitiondata.musicvolumetween = Tweening<float>(getCurrentSong()->getVolume(), 0.f, menutransitiondata.speed);
		}
		else {
			if (newmenu == "exit") {
				w->close();
				return;
			}
			if (newmenu != "game") {
				menuscreens[newmenu]->start();
			}
			else if (newmenu == "game") getCurrentRoom()->start();
			currentmenu = newmenu;
			menutransitiondata.uppersquaretween = Tweening<vec2>(vec2(in->getScreenSize().x, in->getScreenSize().y / 2), vec2(in->getScreenSize().x, 0), menutransitiondata.speed);
			menutransitiondata.bottomsquaretween = Tweening<vec2>(vec2(in->getScreenSize().x, -in->getScreenSize().y / 2), vec2(in->getScreenSize().x, 0), menutransitiondata.speed);
			menutransitiondata.musicvolumetween = Tweening<float>(0.f, menutransitiondata.oldvolume, menutransitiondata.speed);
			menutransitiondata.actually_finished = true;
			setMenuTransitionOffset(newmenu);
		}
	}
}

void RoomManager::setMenuTransitionOffset(std::string menu) {
	sf::View v = w->getView();
	vec2 offsetU = vec2();
	vec2 offsetB = vec2(0, in->getScreenSize().y);
	if (menu == "game") {
		offsetU = v.getCenter() - v.getSize() / 2.f;
		offsetB += offsetU;
	}
	else {
		v.setCenter(in->getScreenSize() / 2.f);
	}
	w->setView(v);
	menutransitiondata.uppersquare.setPosition(offsetU);
	menutransitiondata.bottomsquare.setPosition(offsetB);
}

void RoomManager::drawMenuTransition() {
	w->draw(menutransitiondata.uppersquare);
	w->draw(menutransitiondata.bottomsquare);
}
