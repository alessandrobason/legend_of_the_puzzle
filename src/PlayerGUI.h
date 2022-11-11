#pragma once
#include <iostream>
// #include "SFML/Graphics.hpp"
#include "Framework/Tweening.h"

class PlayerGUI {
	struct Bar {
		sf::RectangleShape shape;
		Tweening<float> tween;
		vec2 offset;
		vec2 size;
	};
	Bar lifebar;
	Bar expbar;
	sf::Sprite bar;
	vec2 baroffset = vec2(88, 1);
	sf::Color colors[10] = {
		sf::Color(32, 180, 74),
		sf::Color(64, 213, 20),
		sf::Color(109, 217, 21),
		sf::Color(172, 229, 14),
		sf::Color(232, 237, 5),
		sf::Color(253, 197, 0),
		sf::Color(255, 128, 0),
		sf::Color(255, 96, 0),
		sf::Color(255, 57, 1),
		sf::Color(255, 0, 41)
	};
	sf::Color expbarcolor = sf::Color(32, 136, 248);
	float remainexp = -1.f;
	float nextlevelexp = 0.f;
	float totallife = 0.f;

public:

	PlayerGUI() {
		lifebar.size = vec2(48, 2);
		lifebar.offset = vec2(104, 3);
		expbar.size = vec2(35, 2);
		expbar.offset = vec2(117, 11);

		lifebar.shape.setFillColor(colors[0]);
		lifebar.shape.setSize(lifebar.size);
		expbar.shape.setFillColor(expbarcolor);
		expbar.shape.setSize(vec2(0, expbar.size.y));
	}

	void setBarTexture(sf::Texture* t) {
		bar.setTexture(*t);
	}

	void setTotalLife(float tl) { totallife = tl; }
	void setNextLevelExp(float exp) { nextlevelexp = exp; }

	bool isFinished() { return lifebar.tween.isfinished(); }

	void reset() { 
		updateLifeValue(totallife); 
		updateExpValue(0);
	}

	void updateLifeValue(float life) {
		float life_totallife = life / totallife;
		lifebar.tween = Tweening<float>(lifebar.shape.getSize().x, life_totallife * lifebar.size.x, 0.2f);
		sf::Color c = colors[(int)((1.f - life_totallife) * 10)];
		lifebar.shape.setFillColor(c);
		lifebar.shape.setOutlineColor(c);
	}

	void updateExpValue(float exp) {
		float exp_totalexp = exp / nextlevelexp;
		std::cout << "exp: " << exp << " nextlevel: " << nextlevelexp << " // " << exp_totalexp << "\n";
		expbar.tween = Tweening<float>(expbar.shape.getSize().x, exp_totalexp * expbar.size.x, 0.2f);
	}

	void remainExpValue(float exp) {
		remainexp = exp;
	}

	void update(float dt) {
		if (!lifebar.tween.isfinished()) {
			lifebar.tween.update(dt);
			lifebar.shape.setSize(vec2(lifebar.tween.getValue(), lifebar.size.y));
		}
		if (!expbar.tween.isfinished()) {
			expbar.tween.update(dt);
			expbar.shape.setSize(vec2(expbar.tween.getValue(), expbar.size.y));
			if (expbar.tween.isfinished() && remainexp >= 0.f) {
				expbar.shape.setSize(vec2(0, expbar.size.y));
				updateExpValue(remainexp);
				remainexp = -1.f;
			}
		}
	}

	void draw(sf::RenderWindow* w) {
		vec2 p = w->mapPixelToCoords(vec2i());

		if (w->getView().getViewport().width != 1.f || w->getView().getViewport().height != 1.f) {
			vec2 viewsize = w->getView().getSize();
			vec2 actualviewsize = vec2(w->getSize().x * w->getView().getViewport().width, w->getSize().y * w->getView().getViewport().height);
			vec2 actualcamerasize = (vec2)w->getSize();
			vec2 camerasize = vec2(viewsize.x * actualcamerasize.x / actualviewsize.x, viewsize.y * actualcamerasize.y / actualviewsize.y);
			vec2 cameraoffset = camerasize - viewsize;

			p += (cameraoffset / 2.f);
		}

		bar.setPosition(p + baroffset);
		lifebar.shape.setPosition(p + lifebar.offset);
		expbar.shape.setPosition(p + expbar.offset);
		w->draw(bar);
		w->draw(lifebar.shape);
		w->draw(expbar.shape);
	}
};

