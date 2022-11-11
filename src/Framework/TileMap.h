#pragma once

#include <raylib.h>
#include <iostream>
#include <vector>
#include "Collision.h"
#include "Animation.h"
#include "../maths.h"

class Tilemap {
public:
	struct tilemap_data {
		int numb_layers;
		vec2u tileSize;
		int w;
		int h;
	};

	struct animated_tiles_data {
		// std::string name;
		int id;
		struct data {
			int duration;
			int tileid;
		};
		std::vector<data> frames;
	};

	// void setWindow(sf::RenderWindow* w);

	void setTexture(const Texture &t);
	Texture* getTexture() { return &m_tileset; }

	bool load(bool* d, Image& t, const std::vector<animated_tiles_data> &animData, std::vector<int>& tiles, const tilemap_data &tilemapdata, vec2 offset);
	bool loadCollisions(std::vector<Collision> &&collisions);

	void draw();
	void animate(float dt);
	void drawDebug();
	void setShader(Shader s);

	tilemap_data getData() { return m_tilemap; }
	// sf::RenderStates getStates() { return states; }
	// vec2 getPosition() { return vec2(states.transform.getMatrix()[12], states.transform.getMatrix()[1]); }

	// void setStates(sf::RenderStates s) { states = s; }

	// void resetAnimation() { animatedTiles.reset(); }

	std::vector<Collision> collisions;

	~Tilemap() {
		UnloadTexture(m_tileset);
	}

private:
	struct TileAnimated {
		int anim_id = 0;
		vec2 pos = {};
		TileAnimated() = default;
		TileAnimated(int anim_id, vec2 pos) : anim_id(anim_id), pos(pos) {}
	};

	Texture m_tileset;
	tilemap_data m_tilemap;
	std::vector<int> m_tiles;
	std::vector<TileAnimated> m_animated_tiles;
	std::vector<AnimatedTile> m_tile_animations;
	vec2 m_offset;

	// sf::RenderStates states;

	bool* isdebug = nullptr;
};
