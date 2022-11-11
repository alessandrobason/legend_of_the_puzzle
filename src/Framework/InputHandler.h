#pragma once
// #include <SFML/Graphics.hpp>
#include <iostream>
#include "../maths.h"

enum Keys : int {
	KeyUp = KEY_W,
	KeyLeft = KEY_A,
	KeyRight = KEY_D,
	KeyDown = KEY_S,
	KeyInteract = KEY_E,
	KeyDebug = KEY_F1
};

extern vec2i resolution;
extern vec2i screensize;

#if 0
class InputHandler
{
public:
	InputHandler(sf::RenderWindow* win) { w = win; }

	void setKeyDown(int key) { if(key == -1) return; keys[key] = true; };
	void setKeyUp(int key) { if (key == -1) return; keys[key] = false; pressed_keys[key] = false; };
	bool isKeyDown(int key) { if (key == -1) return false; return keys[key]; };
	bool isKeyPressed(int key) {
		if (key == -1) return false;
		if (keys[key] && !pressed_keys[key]) {
			pressed_keys[key] = true;
			return true;
		}
		return false;
	}

	void setMouseX(int lx) { mouse.position.x = lx; };
	void setMouseY(int ly) { mouse.position.y = ly; };
	void setMousePosition(vec2i pos) { mouse.position = pos; };
	int getMouseX() { return mouse.position.x; };
	int getMouseY() { return mouse.position.y; };
	vec2i getMousePosition() { return mouse.position; };
	vec2 getMouseRelative() { return w->mapPixelToCoords(sf::Mouse::getPosition(*w)); }
	void setMouseLDown() { mouse.left = true; };
	void setMouseRDown() { mouse.right = true; };
	void setMouseLUp() { mouse.left = false;  mouse.pressedleft = false; };
	void setMouseRUp() { mouse.right = false; mouse.pressedright = false; };
	bool isMouseLDown() { return mouse.left; };
	bool isMouseRDown() { return mouse.right; };
	bool isMouseLPressed() {
		if (mouse.left && !mouse.pressedleft) {
			mouse.pressedleft = true;
			return true;
		}
		return false;
	}
	bool isMouseRPressed() {
		if (mouse.right && !mouse.pressedright) {
			mouse.pressedright = true;
			return true;
		}
		return false;
	}

	void resizeView(vec2u windowsize) {
		rect viewport(0.f, 0.f, 1.f, 1.f);

		vec2 screensize = { (float)GetScreenWidth(), (float)GetScreenHeight() };
		
		float screenwidth = static_cast<float>(windowsize.x) / screensize.x;
		float screenheight = static_cast<float>(windowsize.y) / screensize.y;

		if (screenwidth > screenheight) {
			viewport.w = screenheight / screenwidth;
			viewport.x = (1.f - viewport.w) / 2.f;
		}
		else if (screenwidth < screenheight) {
			viewport.h = screenwidth / screenheight;
			viewport.y = (1.f - viewport.h) / 2.f;
		}
		// sf::View view(rect(0, 0, static_cast<float>(screensize.x), static_cast<float>(screensize.y)));
		// view.setViewport(viewport);
		// windowview = view;
	}

	// sf::View getView() { return windowview; }
	// void setScreenSize(vec2 s) { const_cast<vec2&>(screensize) = s; windowview.setSize(s); }
	// const vec2 getScreenSize() { return screensize; }

	bool isfullscreen = false;

private:
	sf::RenderWindow* w = nullptr;
	// gameboy screen size
	const vec2 screensize = vec2(0, 0);

	struct Mouse {
		vec2i position;
		bool left = false;
		bool right = false;
		bool pressedleft = false;
		bool pressedright = false;
	};

	bool keys[sf::Keyboard::KeyCount]{ false };
	bool pressed_keys[sf::Keyboard::KeyCount]{ false };
	Mouse mouse;

	sf::View windowview = sf::View(rect(0, 0, screensize.x, screensize.y));
};
#endif
