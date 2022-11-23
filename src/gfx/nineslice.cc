#include "nineslice.h"

#include <system.h>

static void DrawTextureTiled(Texture2D texture, vec2i tilesize, const recti &dest);

NineSlice::NineSlice(const char *filename) {
    texture = sys::loadTexture(filename);
    dst = { 0, 0, texture.width, texture.height };
}

NineSlice::NineSlice(const char *filename, const recti &new_dst) {
    texture = sys::loadTexture(filename);
    dst = new_dst;
}

void NineSlice::draw() {
    Node::draw();

    vec2i tex_size = { texture.width, texture.height };
    DrawTextureTiled(texture, tex_size / 3, dst);
}

static void DrawTextureTiled(Texture2D texture, vec2i tilesize, const recti &dest) {
    // draw corners
    // top-left
    DrawTexturePro(
        texture, 
        recti{ vec2i(0), tilesize },
        recti{ dest.pos, tilesize }, 
        vec2(), 0.f, WHITE
    );
    // top-right
    DrawTexturePro(
        texture, 
        recti{ vec2i(tilesize.x * 2, 0), tilesize },
        recti{ vec2i(dest.pos.x + dest.size.x - tilesize.x, dest.pos.y), tilesize }, 
        vec2(), 0.f, WHITE
    );
    // bottom-left
    DrawTexturePro(
        texture, 
        recti{ vec2i(0, tilesize.y * 2), tilesize },
        recti{ vec2i(dest.pos.x, dest.pos.y + dest.size.y - tilesize.y), tilesize }, 
        vec2(), 0.f, WHITE
    );
    // bottom-right
    DrawTexturePro(
        texture, 
        recti{ tilesize * 2, tilesize },
        recti{ dest.pos + dest.size - tilesize, tilesize }, 
        vec2(), 0.f, WHITE
    );

    // draw centre
    DrawTexturePro(
        texture, 
        recti{ tilesize, tilesize },
        recti{ dest.pos + tilesize, dest.size - tilesize * 2 }, 
        vec2(), 0.f, WHITE
    );

    // draw sides
    vec2i count = dest.size / tilesize - 1;
    // left
    for (int i = 1; i < count.y; ++i) {
        DrawTexturePro(
            texture,
            recti{ vec2i(0, tilesize.y), tilesize },
            recti{ dest.pos + vec2i(0, tilesize.y * i), tilesize },
            vec2(), 0.f, WHITE
        );
    }
    // right
    for (int i = 1; i < count.y; ++i) {
        DrawTexturePro(
            texture,
            recti{ vec2i(tilesize.x * 2, tilesize.y), tilesize },
            recti{ dest.pos + vec2i(dest.size.x - tilesize.x, tilesize.y * i), tilesize },
            vec2(), 0.f, WHITE
        );
    }
    // top
    for (int i = 1; i < count.x; ++i) {
        DrawTexturePro(
            texture,
            recti{ vec2i(tilesize.x, 0), tilesize },
            recti{ dest.pos + vec2i(tilesize.x * i, 0), tilesize },
            vec2(), 0.f, WHITE
        );
    }
    // bottom
    for (int i = 1; i < count.x; ++i) {
        DrawTexturePro(
            texture,
            recti{ vec2i(tilesize.x, tilesize.y * 2), tilesize },
            recti{ dest.pos + vec2i(tilesize.x * i, dest.size.y - tilesize.y), tilesize },
            vec2(), 0.f, WHITE
        );
    }
}
