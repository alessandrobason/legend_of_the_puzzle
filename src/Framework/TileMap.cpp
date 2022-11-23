#include "Tilemap.h"

bool Tilemap::load(bool* d, sf::Image* t, std::vector <animated_tiles_data> animData, std::vector<int>& tiles, tilemap_data tilemapdata, vec2 offset) {
	isdebug = d;

	sf::Transform positionoffset;
	positionoffset.translate(offset);
	states.transform = positionoffset;
	
	m_tilemap = tilemapdata;
	
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

						animatedTiles.emplace_back((int)m_tile_animations.size(), tile_pos);
						m_tile_animations.emplace_back(std::move(new_anim));
					}
					else {
						animatedTiles.emplace_back(animation_id, tile_pos);
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

#if 0
	// set the size of the vertex array to be enough for the whole tilemap
	const int layer_dimension = (int) tiles.size() / tilemapdata.numb_layers;
	
	/*
	modified version of the official sfml tutorial for vertex arrays
	*/

	for (int nl = 0; nl < tilemapdata.numb_layers; nl++) {
		for (int y = 0; y < tilemapdata.h; y++) {
			for (int x = 0; x < tilemapdata.w; x++) {
				int position = (nl * layer_dimension) + (y * tilemapdata.w) + (x);
				
				// get tile
				int tileNumber = tiles[position];

				// if tile is empty
				if (tileNumber == -1) continue;

				bool isAnimated = false;
				int positionInAnimData = -1;
				
				std::string tileStr = std::to_string(tileNumber);
				for (size_t i = 0; i < animData.size(); i++) {
					if (animData[i].name == tileStr) {
						isAnimated = true;
						positionInAnimData = (int)i;
						break;
					}
				}
				

				// get position in texture
				int tu = tileNumber % (t->getSize().x / tilemapdata.tileSize.x);
				int tv = tileNumber / (t->getSize().x / tilemapdata.tileSize.x);


				if (isAnimated) {
					sf::Vertex quad[4];

					// set position of 4 vertices (cw order)
					quad[0].position = vec2i(x * tilemapdata.tileSize.x, y * tilemapdata.tileSize.y);
					quad[1].position = vec2i((x + 1) * tilemapdata.tileSize.x, y * tilemapdata.tileSize.y);
					quad[2].position = vec2i((x + 1) * tilemapdata.tileSize.x, (y + 1) * tilemapdata.tileSize.y);
					quad[3].position = vec2i(x * tilemapdata.tileSize.x, (y + 1) * tilemapdata.tileSize.y);
					// set texture coords (cw order)
					quad[0].texCoords = vec2i(tu * tilemapdata.tileSize.x, tv * tilemapdata.tileSize.y);
					quad[1].texCoords = vec2i((tu + 1) * tilemapdata.tileSize.x, tv * tilemapdata.tileSize.y);
					quad[2].texCoords = vec2i((tu + 1) * tilemapdata.tileSize.x, (tv + 1) * tilemapdata.tileSize.y);
					quad[3].texCoords = vec2i(tu * tilemapdata.tileSize.x, (tv + 1) * tilemapdata.tileSize.y);
					
					std::vector<recti> frames;
					int speed = animData[positionInAnimData].frames[0].duration;

					for (size_t i = 0; i < animData[positionInAnimData].frames.size(); i++) {
						int fu = animData[positionInAnimData].frames[i].tileid % (t->getSize().x / tilemapdata.tileSize.x);
						int fv = animData[positionInAnimData].frames[i].tileid / (t->getSize().x / tilemapdata.tileSize.x);
						int fx = fu * tilemapdata.tileSize.x;
						int fy = fv * tilemapdata.tileSize.y;
						int fw = (fu + 1) * tilemapdata.tileSize.x;
						int fh = (fv + 1) * tilemapdata.tileSize.y;
						frames.push_back(recti(fx, fy, fw, fh));
					}

					animatedTiles.appendTile(quad, frames, speed);
				}
				else {
					// pointer to the tile's quad
					sf::Vertex quad[4];// = &vertexs[position * 4];

					// set position of 4 vertices (cw order)
					quad[0].position = vec2i(x * tilemapdata.tileSize.x, y * tilemapdata.tileSize.y);
					quad[1].position = vec2i((x + 1) * tilemapdata.tileSize.x, y * tilemapdata.tileSize.y);
					quad[2].position = vec2i((x + 1) * tilemapdata.tileSize.x, (y + 1) * tilemapdata.tileSize.y);
					quad[3].position = vec2i(x * tilemapdata.tileSize.x, (y + 1) * tilemapdata.tileSize.y);

					// set texture coords (cw order)
					quad[0].texCoords = vec2i(tu * tilemapdata.tileSize.x, tv * tilemapdata.tileSize.y);
					quad[1].texCoords = vec2i((tu + 1) * tilemapdata.tileSize.x, tv * tilemapdata.tileSize.y);
					quad[2].texCoords = vec2i((tu + 1) * tilemapdata.tileSize.x, (tv + 1) * tilemapdata.tileSize.y);
					quad[3].texCoords = vec2i(tu * tilemapdata.tileSize.x, (tv + 1) * tilemapdata.tileSize.y);
					
					vertexs.push_back(quad[0]);
					vertexs.push_back(quad[1]);
					vertexs.push_back(quad[2]);
					vertexs.push_back(quad[3]);
				}
			}
		}
	}

	return true;
#endif
}

void Tilemap::setTexture(sf::Texture* t) {
	m_tileset = *t;
	states.texture = t;
	// animatedTiles.setTilemap(t);
}

bool Tilemap::loadCollisions(std::vector<Collision> c) {
	collisions = c;
	return true;
}

void Tilemap::setWindow(sf::RenderWindow* w){
	window = w;
}

void Tilemap::animate(float dt) {
	for (auto &anim : m_tile_animations) {
		anim.update(dt);
	}
	// animatedTiles.animate(dt);
}

void Tilemap::draw(){
	// window->draw(vertexs.data(), vertexs.size(), sf::Quads, states);
	// window->draw(animatedTiles, states);
	
	vec2i tileset_sz = m_tileset.getSize();
	int layer_dimension = m_tilemap.w * m_tilemap.h;
	int width_in_tiles = tileset_sz.x / m_tilemap.tileSize.x;

	for (int l = 0; l < m_tilemap.numb_layers; ++l) {
		for (int y = 0; y < m_tilemap.h; y++) {
			for (int x = 0; x < m_tilemap.w; x++) {
				int index = (l * layer_dimension) + (y * m_tilemap.w) + x;
				int tile = m_tiles[index];
				if (tile == -1) {
					continue;
				}

				rectf sub = {
					(float)(tile % width_in_tiles * m_tilemap.tileSize.x),
					(float)(tile / width_in_tiles * m_tilemap.tileSize.y),
					(float)(m_tilemap.tileSize.x),
					(float)(m_tilemap.tileSize.y)
				};

				vec2 pos = m_offset + vec2((float)x, (float)y) * m_tilemap.tileSize;
				rl::DrawTextureRec(m_tileset, sub, pos, rl::WHITE);
			}
		}
	}

	for (TileAnimated &anim_tile : animatedTiles) {
		if (anim_tile.anim_id < 0) {
			continue;
		}
		AnimatedTile &animation = m_tile_animations[anim_tile.anim_id];
		int tile = animation.getFrame();
		vec2 pos = m_offset + anim_tile.pos;
		rectf sub = {
			(float)(tile % width_in_tiles * m_tilemap.tileSize.x),
			(float)(tile / width_in_tiles * m_tilemap.tileSize.y),
			(float)(m_tilemap.tileSize.x),
			(float)(m_tilemap.tileSize.y)
		};
		rl::DrawTextureRec(m_tileset, sub, pos, rl::WHITE);
	}
}

void Tilemap::drawDebug() {
	for (size_t i = 0; i < collisions.size(); i++) {
		collisions[i].drawDebug(window);
	}
}

void Tilemap::setShader(sf::Shader s){
	states.shader = &s;
}
