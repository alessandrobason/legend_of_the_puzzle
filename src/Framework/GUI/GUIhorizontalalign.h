#pragma once
#include "GUIelement.h"

class GUIhorizontalalign : public GUIelement {
	std::vector<GUIelement*> elements;

public:
	void setBackgroundColor(sf::Color bc) { backgroundcolor = bc; }

	void addElement(GUIelement* e) { elements.push_back(e); e->setParent(this); }

	void load() {
		sf::Vertex v[4];
		v[0].position = vec2(boxrect.left, boxrect.top);
		v[1].position = vec2(boxrect.left + boxrect.width, boxrect.top);
		v[2].position = vec2(boxrect.left + boxrect.width, boxrect.top + boxrect.height);
		v[3].position = vec2(boxrect.left, boxrect.top + boxrect.height);
		v[0].color = backgroundcolor;
		v[1].color = backgroundcolor;
		v[2].color = backgroundcolor;
		v[3].color = backgroundcolor;
		vertexs.push_back(v[0]);
		vertexs.push_back(v[1]);
		vertexs.push_back(v[2]);
		vertexs.push_back(v[3]);
		alignElement();
	}

	void alignElements() {
		float totalwidth = 0.f;
		for (size_t i = 0; i < elements.size(); i++) {
			totalwidth += elements[i]->getRect().width;
		}
		float spacebetween = (boxrect.width - totalwidth) / (elements.size() + 1);
		float x = 0;
		for (size_t i = 0; i < elements.size(); i++) {
			x += spacebetween;
			GUIelement* e = elements[i];
			e->setPosition(vec2(x, e->getPosition().y));
			x += e->getRect().width;
			e->setAlign(GUIelement::ALIGN::NONE, GUIelement::ALIGN::CENTER);
			e->alignElement();
		}
	}

	void setPosition(vec2 pos) override {
		vec2 difference = pos - getPosition();
		GUIelement::setPosition(pos);
		for (size_t i = 0; i < elements.size(); i++) {
			GUIelement* e = elements[i];
			e->setPosition(e->getPosition() + difference);
		}
	}

	void draw(sf::RenderWindow* w) override {
		sf::RectangleShape h;
		h.setPosition(vec2(getPosition().x, getPosition().y));
		h.setSize(vec2(boxrect.width, boxrect.height));
		h.setFillColor(sf::Color::Transparent);
		h.setOutlineColor(sf::Color::Red);
		h.setOutlineThickness(1.f);
		w->draw(h);
	}
};