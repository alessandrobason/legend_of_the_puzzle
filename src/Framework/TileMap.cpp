#include "Tilemap.h"

bool Tilemap::load(
	bool* d,
	Image& t, 
	const std::vector<animated_tiles_data> &animData, 
	std::vector<int>& tiles, 
	const tilemap_data &tilemapdata, 
	vec2 offset
) {
	isdebug = d;
	m_tilemap = tilemapdata;
	m_offset = offset;
	m_tileset = LoadTextureFromImage(t);

	// sf::Transform positionoffset;
	// positionoffset.translate(offset);
	// states.transform = positionoffset;
	
	// set the size of the vertex array to be enough for the whole tilemap
	const int layer_dimension = (int) tiles.size() / m_tilemap.numb_layers;

	for (int l = 0; l < m_tilemap.numb_layers; ++l) {
		for (int y = 0; y < m_tilemap.h; y++) {
			for (int x = 0; x < m_tilemap.w; x++) {
				int index = (l * layer_dimension) + (y * m_tilemap.w) + x;
				
				// get tile
				int tile = tiles[index];
				bool isAnimated = false;
				int animId = 0;

				if (tile != -1) {
					for (size_t i = 0; i < animData.size(); i++) {
						if (animData[i].id == tile) {
							isAnimated = true;
							animId = (int)i;
							break;
						}
					}
				}

				if (isAnimated) {
					int animation_id = -1;
					vec2 tile_pos = vec2((float)x, (float)y) * m_tilemap.tileSize;

					for (size_t i = 0; i < m_tile_animations.size(); ++i) {
						if (m_tile_animations[i].tile_id == tile) {
							animation_id = (int)i;
							break;
						}
					}

					if (animation_id < 0) {
						AnimatedTile new_anim;
						const auto &anim = animData[animId];
						new_anim.duration = (float)anim.frames[0].duration / 1000.f;

						for (const auto &frame : anim.frames) {
							new_anim.frames.emplace_back(frame.tileid);
						}

						m_animated_tiles.emplace_back((int)m_tile_animations.size(), tile_pos);
						m_tile_animations.emplace_back(std::move(new_anim));
					}
					else {
						m_animated_tiles.emplace_back(animation_id, tile_pos);
					}

					m_tiles.emplace_back(-1);
				}
				else {
					m_tiles.emplace_back(tile);
				}
			}
		}
	}
	return true;
}

void Tilemap::setTexture(const Texture &t) {
	m_tileset = t;
}

bool Tilemap::loadCollisions(std::vector<Collision> &&c) {
	collisions = std::move(c);
	return true;
}

void Tilemap::animate(float dt) {
	for (auto &anim : m_tile_animations) {
		anim.update(dt);
	}
}

void Tilemap::draw(){
	int layer_dimension = m_tilemap.w * m_tilemap.h;
	int width_in_tiles = m_tileset.width / m_tilemap.tileSize.x;

	for (int l = 0; l < m_tilemap.numb_layers; ++l) {
		for (int y = 0; y < m_tilemap.h; y++) {
			for (int x = 0; x < m_tilemap.w; x++) {
				int index = (l * layer_dimension) + (y * m_tilemap.w) + x;
				int tile = m_tiles[index];
				if (tile == -1) {
					continue;
				}

				rect sub = {
					(float)(tile % width_in_tiles * m_tilemap.tileSize.x),
					(float)(tile / width_in_tiles * m_tilemap.tileSize.y),
					(float)(m_tilemap.tileSize.x),
					(float)(m_tilemap.tileSize.y)
				};

				vec2 pos = m_offset + vec2((float)x, (float)y) * m_tilemap.tileSize;
				DrawTextureRec(m_tileset, sub, pos, WHITE);
			}
		}
	}

	for (TileAnimated &anim_tile : m_animated_tiles) {
		if (anim_tile.anim_id < 0) {
			continue;
		}
		AnimatedTile &animation = m_tile_animations[anim_tile.anim_id];
		int tile = animation.getFrame();
		vec2 pos = m_offset + anim_tile.pos;
		rect sub = {
			(float)(tile % width_in_tiles * m_tilemap.tileSize.x),
			(float)(tile / width_in_tiles * m_tilemap.tileSize.y),
			(float)(m_tilemap.tileSize.x),
			(float)(m_tilemap.tileSize.y)
		};
		DrawTextureRec(m_tileset, sub, pos, WHITE);
	}
}

void Tilemap::drawDebug() {
	for (auto &c : collisions) {
		c.drawDebug();
	}
}

void Tilemap::setShader(Shader s){
	// states.shader = &s;
}