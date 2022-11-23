#include "GUItiledelement.h"

void GUItiledelement::load() {
	if (tilesize == NULL) {
		std::cout << "tilesize can't be 0\n";
		abort();
	}
}

void GUItiledelement::appendQuad(sf::Vertex v, vec2 size) {
	// if (states.texture) {
	// 	quads.emplace_back(
	// 		rectf(v.texCoords, v.texCoords + (float)tilesize), 
	// 		rectf(v.position, size), 
	// 		v.color
	// 	);
	// }
	// else {
	// 	quads.emplace_back(
	// 		rectf(0, 0, 0, 0), 
	// 		rectf(v.position, size), 
	// 		backgroundcolor
	// 	);
	// }
	// return;

	vec2 coords = v.position;
	vec2 texcoords = v.texCoords;
	vec2 positionoffset[4] = {
		vec2(0, 0),
		vec2(size.x, 0),
		vec2(size.x, size.y),
		vec2(0, size.y),
	};
	vec2 textureoffset[4] = {
		vec2(0, 			  0),
		vec2((float)tilesize, 0),
		vec2((float)tilesize, (float)tilesize),
		vec2(0,               (float)tilesize),
	};

	rectf dst(coords, size);
	rectf src(texcoords, vec2((float)tilesize));
	quads.emplace_back(src, dst, v.color);

	// for (size_t i = 0; i < 4; i++) {
	// 	v.position = positionoffset[i] + coords;
	// 	v.texCoords = textureoffset[i] + texcoords;
	// 	vertexs.push_back(v);
	// }
}

void GUItiledelement::nineslice() {
	sf::Vertex v;
	if (backgroundcolor != sf::Color::Transparent) v.color = backgroundcolor;
	rectf boxrectnine = boxrect;
	boxrectnine.width += boxrect.left;
	boxrectnine.height += boxrect.top;

	// LOAD MIDDLE
	v.position = vec2(boxrectnine.left + tilesize, boxrectnine.top + tilesize);
	v.texCoords = vec2i(tilesize, tilesize);
	appendQuad(v, vec2(boxrect.width - (tilesize * 2), boxrect.height - (tilesize * 2)));

	// LOAD CORNERS
	vec2 corner[4] = {
		vec2(boxrectnine.left			 , boxrectnine.top),
		vec2(boxrectnine.width - tilesize, boxrectnine.top),
		vec2(boxrectnine.left			 , boxrectnine.height - tilesize),
		vec2(boxrectnine.width - tilesize, boxrectnine.height - tilesize)
	};
	vec2 texCorner[4] = {
		vec2i(0			 , 0),
		vec2i(tilesize * 2, 0),
		vec2i(0			 , tilesize * 2),
		vec2i(tilesize * 2, tilesize * 2)
	};
	vec2 coords[4] = {
		vec2i(0		  , 0),
		vec2i(tilesize, 0),
		vec2i(tilesize, tilesize),
		vec2i(0		  , tilesize),
	};

	for (size_t i = 0; i < 4; i++) {
		v.position = corner[i];
		v.texCoords = texCorner[i];
		appendQuad(v, vec2i(tilesize, tilesize));
	}


	// LOAD INBETWEENS
	int totalwidth = (int)boxrect.width - tilesize * 2;
	int totalheight = (int)boxrect.height - tilesize * 2;
	numberoftiles = vec2i(totalwidth, totalheight) / tilesize;
	remainingsize = vec2i(totalwidth % tilesize, totalheight % tilesize);
	std::cout << "###### REMAINING SIZE ######" << remainingsize.x << " " << remainingsize.y << "\n";

	vec2 sideCoords[4]{
		vec2(boxrectnine.left + tilesize , boxrectnine.top),
		vec2(boxrectnine.left + tilesize , boxrectnine.height - tilesize),
		vec2(boxrectnine.left			 , boxrectnine.top + tilesize),
		vec2(boxrectnine.width - tilesize, boxrectnine.top + tilesize)
	};
	vec2 sideTex[4]{
		vec2i(tilesize  , 0),
		vec2i(tilesize  , tilesize * 2),
		vec2i(0		   , tilesize),
		vec2i(tilesize * 2, tilesize)
	};

	for (size_t i = 0; i < 2; i++) {
		for (size_t j = 0; j < numberoftiles.x; j++) {
			// HORIZONTAL
			float x = sideCoords[i].x + (tilesize * j);
			v.position = vec2(x, sideCoords[i].y);
			v.texCoords = sideTex[i];
			appendQuad(v, vec2i(tilesize, tilesize));
		}
		v.position = vec2(sideCoords[i].x + numberoftiles.x * tilesize, sideCoords[i].y);
		v.texCoords = sideTex[i];
		appendQuad(v, vec2(remainingsize.x, (float)tilesize));
		//////////////////////////////////////////////
		for (size_t j = 0; j < numberoftiles.y; j++) {
			// VERTICAL
			float y = sideCoords[i + 2].y + (tilesize * j);
			v.position = vec2(sideCoords[i + 2].x, y);
			v.texCoords = sideTex[i + 2];
			appendQuad(v, vec2i(tilesize, tilesize));
		}
		v.position = vec2(sideCoords[i + 2].x, sideCoords[i + 2].y + numberoftiles.y * tilesize);
		v.texCoords = sideTex[i + 2];
		appendQuad(v, vec2((float)tilesize, remainingsize.y));
	}
}
