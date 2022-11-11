// #include <SFML/Graphics.hpp>
#include <raylib.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "maths.h"
#include "camera.h"
#include "Framework/RoomManager.h"

#if 0

float min(float a, float b) { return (a < b ? a : b); }
float max(float a, float b) { return (a > b ? a : b); }
bool pressed = false;

void windowProcess(sf::RenderWindow* window, InputHandler *in) {
	sf::Event event;
	while (window->pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::Resized:
			in->resizeView(vec2u(event.size.width, event.size.height));
			break;
		case sf::Event::KeyPressed:
			in->setKeyDown(event.key.code);
			break;
		case sf::Event::KeyReleased:
			in->setKeyUp(event.key.code);
			break;
		case sf::Event::MouseMoved:
			in->setMousePosition(sf::Mouse::getPosition());
			break;
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left)
				in->setMouseLDown();
			if (event.mouseButton.button == sf::Mouse::Right)
				in->setMouseRDown();
			break;
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left)
				in->setMouseLUp();
			if (event.mouseButton.button == sf::Mouse::Right)
				in->setMouseRUp();
			break;
		default:
			break;
		}
	}
}
#endif

int zoom = 5;
vec2i resolution = { 160, 144 };
vec2i screensize = resolution * zoom;

int main() {
	srand((unsigned int)time(NULL));

	InitWindow(screensize.x, screensize.y, "The Legend of the Puzzle");
    SetTargetFPS(60);

	g_view.zoom = (float)zoom;

	RoomManager roommanager{};
	roommanager.init();
	// roommanager.setData(&window, in);
	while (!WindowShouldClose()) {
		float dt = GetFrameTime();
		roommanager.handleInput(dt);
		roommanager.update(dt);

        BeginDrawing();
			BeginMode2D(g_view);
				roommanager.draw();
			EndMode2D();
        EndDrawing();
	}
}

