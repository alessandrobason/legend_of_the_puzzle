#include "gui.h"

#include "system.h"
#include "tracelog.h"

Button::Button(const char *new_text, vec2i pos, vec2i size, int new_id, CallBack cb, void *udata) {
    dst = { pos, size };

    id = new_id;
    userdata = udata;
    callback = cb;

    text = new_text;
    vec2 text_size = text.getSize();
    recti box = dst + recti(2, 4, -3, -12);
    text.pos = box.centre() - text_size / 2.f;
}

void Button::init() {
    textures[Normal]  = sys::loadTexture("assets/GUI/button.png");
    textures[Hover]   = sys::loadTexture("assets/GUI/buttonhover.png");
    textures[Pressed] = sys::loadTexture("assets/GUI/buttonpressed.png");
    info = {
        { 0, 0, 24, 24 },
        3, 21, 4, 17,
        NPATCH_NINE_PATCH
    };
}

void Button::cleanup() {
    for (int i = 0; i < Count; ++i) {
        UnloadTexture(textures[i]);
    }
}

void Button::update() {
    if (disabled) return;
    
    recti box = dst + recti(0, 1, 0, -3);
    bool is_down = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    bool is_inside = box.contains(sys::getMousePos());

    if (!was_down && is_down) {
        was_inside_when_pressed = is_inside;
    }

    switch (state) {
    case Normal:
        if (is_inside) {
            state = Hover;
        }
        break;
    case Hover:
        if (!is_inside) {
            state = Normal;
        }
        else if (is_down && was_inside_when_pressed) {
            state = Pressed;
            text.pos.y += 2;
        }
        break;
    case Pressed:
        if (!is_inside) {
            text.pos.y -= 2;
            state = Normal;
        }
        else if (!is_down) {
            if (was_down && callback) {
                callback(userdata, id);
            }
            state = Hover;
            text.pos.y -= 2;
        }
        break;
    }

    was_down = is_down;

    Node::update();
}

void Button::draw() {
    if (disabled) return;
    DrawTextureNPatch(textures[state], info, dst, vec2(), 0.f, WHITE);
    text.draw();
    Node::draw();
}