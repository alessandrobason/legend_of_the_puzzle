#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class InputHandler
{
public:
	enum KEY_ENUM {
		KEY_UP = sf::Keyboard::W,
		KEY_LEFT = sf::Keyboard::A,
		KEY_RIGHT = sf::Keyboard::D,
		KEY_DOWN = sf::Keyboard::S,
		KEY_INTERACT = sf::Keyboard::E,
		KEY_DEBUG = sf::Keyboard::F1,
		KEY_MENU = sf::Keyboard::Escape,
	};

	InputHandler(sf::RenderWindow* win) { w = win; }

	// void setKeyDown(int key) { if(key == -1) return; keys[key] = true; };
	// void setKeyUp(int key) { if (key == -1) return; keys[key] = false; pressed_keys[key] = false; };
	// bool isKeyDown(int key) { if (key == -1) return false; return keys[key]; };
	bool isKeyDown(KEY_ENUM key) { return sf::Keyboard::isKeyDown((sf::Keyboard::Key)key); };
	bool isKeyPressed(KEY_ENUM key) { return sf::Keyboard::isKeyPressed((sf::Keyboard::Key)key); };
	// bool isKeyPressed(int key) {
	// 	if (key == -1) return false;
	// 	if (keys[key] && !pressed_keys[key]) {
	// 		pressed_keys[key] = true;
	// 		return true;
	// 	}
	// 	return false;
	// }

	// void setMouseX(int lx) { mouse.position.x = lx; };
	// void setMouseY(int ly) { mouse.position.y = ly; };
	// void setMousePosition(vec2i pos) { mouse.position = pos; };
	int getMouseX() { return sf::Mouse::getPosition().x; };
	int getMouseY() { return sf::Mouse::getPosition().y; };
	vec2i getMousePosition() { return sf::Mouse::getPosition(); };
	vec2 getMouseRelative() { return sf::Mouse::getPosition(*w); }
	// void setMouseLDown() { mouse.left = true; };
	// void setMouseRDown() { mouse.right = true; };
	// void setMouseLUp() { mouse.left = false;  mouse.pressedleft = false; };
	// void setMouseRUp() { mouse.right = false; mouse.pressedright = false; };
	bool isMouseLDown() { return sf::Mouse::isButtonDown(sf::Mouse::Left); };
	bool isMouseRDown() { return sf::Mouse::isButtonDown(sf::Mouse::Right); };
	bool isMouseLPressed() { return sf::Mouse::isButtonPressed(sf::Mouse::Left); }
	// 	if (mouse.left && !mouse.pressedleft) {
	// 		mouse.pressedleft = true;
	// 		return true;
	// 	}
	// 	return false;
	// }
	bool isMouseRPressed() { return sf::Mouse::isButtonPressed(sf::Mouse::Right); }
	// 	if (mouse.right && !mouse.pressedright) {
	// 		mouse.pressedright = true;
	// 		return true;
	// 	}
	// 	return false;
	// }


	void resizeView(vec2u windowsize) {
		rectf viewport(0.f, 0.f, 1.f, 1.f);

		float screenwidth = static_cast<float>(windowsize.x) / screensize.x;
		float screenheight = static_cast<float>(windowsize.y) / screensize.y;

		if (screenwidth > screenheight) {
			viewport.width = screenheight / screenwidth;
			viewport.left = (1.f - viewport.width) / 2.f;
		}
		else if (screenwidth < screenheight) {
			viewport.height = screenwidth / screenheight;
			viewport.top = (1.f - viewport.height) / 2.f;
		}

		sf::View view(rectf(0, 0, static_cast<float>(screensize.x), static_cast<float>(screensize.y)));
		view.setViewport(viewport);
		windowview = view;
	}

	sf::View getView() { return windowview; }
	void setScreenSize(vec2 s) { const_cast<vec2&>(screensize) = s; windowview.setSize(s); }
	const vec2 getScreenSize() { return screensize; }

	bool isfullscreen = false;

private:
	sf::RenderWindow* w = nullptr;
	// gameboy screen size
	const vec2 screensize = vec2(0, 0);

	// struct Mouse {
	// 	vec2i position;
	// 	bool left = false;
	// 	bool right = false;
	// 	bool pressedleft = false;
	// 	bool pressedright = false;
	// };

	// bool keys[sf::Keyboard::KeyCount]{ false };
	// bool pressed_keys[sf::Keyboard::KeyCount]{ false };
	// Mouse mouse;

	sf::View windowview = sf::View(rectf(0, 0, screensize.x, screensize.y));
};

