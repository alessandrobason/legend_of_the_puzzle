#pragma once
#include "GUIelement.h"

class GUItiledelement : public GUIelement {
public:
	GUItiledelement() : GUIelement() {}
	GUItiledelement(const GUItiledelement& copy) : GUIelement(copy) {}
	void setTileSize(int t) { tilesize = t; }
	void nineslice();

protected:
	int tilesize = NULL;
	vec2i numberoftiles;
	vec2 remainingsize;
	void load() override;
	void appendQuad(sf::Vertex v, vec2 size);
};