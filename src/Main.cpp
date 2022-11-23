#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Framework/RoomManager.h"
#include "Framework/Room.h"

int main() {
	srand((unsigned int)time(NULL));

	const vec2 screensize = vec2(160, 144);
	sf::RenderWindow window(screensize * 5.f, "Game Project");
	InputHandler *in = new InputHandler(&window);
	in->setScreenSize(screensize);
	window.m_view.setCenter(screensize / 2.f);

	RoomManager roommanager;

	// sf::Clock deltaClock;
	roommanager.setData(&window, in);

	in->resizeView(window.getSize());
	window.setView(in->getView());

	// Game Loop
	while (window.isOpen()) {
		float dt = rl::GetFrameTime();
		// float dt = deltaClock.restart().asSeconds();
		// printf("dt: %.3f\n", dt);
		roommanager.handleInput(dt);
		roommanager.update(dt);
		roommanager.draw();
	}
}
