#pragma once
#include "GUIpanel.h"
#include "../Tweening.h"

class GUIsplashscreen : public GUIpanel {
	Tweening<vec2> tween1;
	Tweening<vec2> tween2;
	Tweening<vec2> tween3;
	Tweening<vec2> animations[3];
	float finalwait = 2.f;

public:

	void setQuadPosition(int i, vec2 p) {
		float w = (float)states.texture->getSize().x;
		float h = (float)states.texture->getSize().y;
		vertexs[i].position = p;
		vertexs[i + 1].position = p + vec2(w, 0);
		vertexs[i + 2].position = p + vec2(w, h);
		vertexs[i + 3].position = p + vec2(0, h);
	}

	void load() {
		if (!states.texture) {
			std::cout << "Splash screen texture not loaded\n";
			abort();
		}
		alignElement();

		sf::Vertex v;
		v.position = vec2(boxrect.left, boxrect.top);
		v.texCoords = vec2();

		// red
		v.color = sf::Color(205, 0, 41);
		appendQuad(v, vec2(boxrect.width, boxrect.height));
		// green
		v.color = sf::Color(32, 180, 74);
		appendQuad(v, vec2(boxrect.width, boxrect.height));
		// white-ish
		v.color = sf::Color(255, 255, 139);
		appendQuad(v, vec2(boxrect.width, boxrect.height));

		float centerx = getPosition().x;
		setPosition(vec2(-boxrect.width, getPosition().y));
		vec2 oldpos = vec2(boxrect.width-10 + centerx, 0);
		animations[0] = Tweening<vec2>(vec2(), vec2(boxrect.width-10 + centerx, 0), 3.f);
		for (size_t i = 1; i < 3; i++) {
			vec2 newpos = oldpos + vec2(10, 0);
			animations[i] = Tweening<vec2>(oldpos, newpos, 0.3f);
			oldpos = newpos;
		}
	}

	void update(float dt) override {
		if (!animations[0].isfinished()) {
			animations[0].update(dt);
			setQuadPosition(0, animations[0].getValue());
			setQuadPosition(4, animations[0].getValue());
			setQuadPosition(8, animations[0].getValue());
		}
		else if (!animations[1].isfinished()) {
			animations[1].update(dt);
			setQuadPosition(4, animations[1].getValue());
			setQuadPosition(8, animations[1].getValue());
		}
		else if (!animations[2].isfinished()) {
			animations[2].update(dt);
			setQuadPosition(8, animations[2].getValue());
		}
		else if (finalwait > 0){
			finalwait -= dt;
		}
		else {
			dead = true;
		}
	}
};
