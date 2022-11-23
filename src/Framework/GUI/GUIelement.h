#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <assert.h>

class GUIelement {
public:
	GUIelement() {}
	GUIelement(const GUIelement& copy);

	enum class CONTROL_STATES {
		NONE,
		HOVER,
		CLICKDOWN,
		CLICKUP
	};

	enum class ALIGN {
		NONE,
		CENTER,
		LEFT,
		RIGHT,
		TOP,
		BOTTOM
	};

	int zbuffer = 0;
	bool enabled = true;
	GUIelement* oldpointer = nullptr;

	virtual void update(float dt) {}
	virtual void draw(sf::RenderWindow* w);

	virtual void trigger() {}
	virtual void load() {}
	virtual void changeControlState(CONTROL_STATES newcontrol) {}

	bool isdead() { return dead; }

	void setParent(GUIelement* p) { parent = p; zbuffer = parent->zbuffer + 1; }
	GUIelement* getParent() { return parent; }

	void setOffset(vec2 o) { elementoffset = o; }
	virtual void setPosition(vec2 pos);
	void setCenter(vec2 p) { setPosition(vec2(p.x - (boxrect.width / 2), p.y - (boxrect.height / 2))); }
	vec2 getPosition() { return vec2(boxrect.left, boxrect.top); }
	vec2 getCenter() { return vec2(boxrect.left, boxrect.top) + (vec2(boxrect.width, boxrect.height) / 2.f); }
	vec2 getOffset() { return elementoffset; }

	void updatePosition(vec2 o = vec2()) {
		if(o != vec2()) setOffset(o);
		setPosition(getPosition());
	}

	virtual int getCharacterSize() { return 0; }
	virtual void setCharacterSize(int s) {}

	virtual void setTexture(sf::Texture* t) { states.texture = t; }
	virtual void updateTexture() {}
	void setRect(rectf b) { boxrect = b; }

	virtual void appendQuad(sf::Vertex v, vec2 size);

	void setAlign(ALIGN h, ALIGN v) { horizontal_alignment = h; vertical_alignment = v; }
	virtual void alignElement();
	
	rectf getRect() { return boxrect; }
	CONTROL_STATES getState() { return currentcontrol; }

	void setShader(std::string file, sf::Shader::Type type);

	void setBackgroundColor(sf::Color bc) { backgroundcolor = bc; }

protected:
	GUIelement* parent = nullptr;

	struct Quad {
		Quad() = default;
		Quad(const rectf &s, const rectf &d, sf::Color c) : src(s), dst(d), col(c) {}
		rectf src;
		rectf dst;
		sf::Color col;
	};

	std::vector<sf::Vertex> vertexs;
	std::vector<Quad> quads;
	sf::RenderStates states;
	rectf boxrect;
	vec2 elementoffset;
	CONTROL_STATES currentcontrol = CONTROL_STATES::NONE;
	ALIGN horizontal_alignment = ALIGN::NONE;
	ALIGN vertical_alignment = ALIGN::NONE;
	sf::Color backgroundcolor = sf::Color::Transparent;
	bool dead = false;

};