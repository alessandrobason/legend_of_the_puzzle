#include "sprite.h"

#include "system.h"

Sprite::Sprite(const char *filename) {
    texture = sys::loadTexture(filename);
    src = {
        0, 0,
        texture.width, texture.height
    };
    dst = src;
}

void Sprite::draw() {
    Node::draw();
    DrawTexturePro(texture, src, dst, origin, angle, color);
}

Atlas::Atlas(const char *filename, vec2i new_pos, vec2i new_count) {
    texture = sys::loadTexture(filename);
    count = new_count;
    pos = new_pos;
}

void Atlas::draw() {
    drawTile(0);
}

void Atlas::drawTile(int x, int y) {
    Node::draw();
    recti dst = getBox();
    recti src = {
        dst.size * vec2i(x, y),
        dst.size
    };
    DrawTexturePro(texture, src, dst, origin, angle, color);
}

recti Atlas::getBox() {
    vec2i size = { texture.width, texture.height };
    vec2i tile_size = size / count;
    return { pos, tile_size };
}