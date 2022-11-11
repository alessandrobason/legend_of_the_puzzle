#pragma once
#include "GUIelement.h"

class GUIpanel : public GUIelement {
public:
	void load() {
		sf::Vertex v;
		v.position = vec2(boxrect.left, boxrect.top);
		appendQuad(v, vec2(boxrect.width, boxrect.height));
	}

	void updateTexture() {
		float w = (float) states.texture->getSize().x;
		float h = (float) states.texture->getSize().y;

		vec2 textureoffset[4] = { 
			vec2(0, 0), vec2(w, 0), vec2(w, h), vec2(0, h)
		};

		vec2 coords = vec2(boxrect.left, boxrect.top);
		vec2 positionoffset[4] = {
			vec2(0, 0),
			vec2(boxrect.width, 0),
			vec2(boxrect.width, boxrect.height),
			vec2(0, boxrect.height),
		};

		for (size_t i = 0; i < 4; i++) {
			sf::Vertex* v = &vertexs[i];
			v->position = coords + positionoffset[i];
			v->texCoords = textureoffset[i];
			v->color = sf::Color::White;
		}
	}
};