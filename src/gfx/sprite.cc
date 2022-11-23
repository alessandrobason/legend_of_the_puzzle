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