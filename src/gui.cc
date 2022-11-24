#include "gui.h"

#include "system.h"
#include "tracelog.h"

static bool is_focused = false;

// == BUTTON ====================================================================================

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
    Node::init();
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
        if (is_inside && !is_focused) {
            state = Hover;
            is_focused = true;
        }
        break;
    case Hover:
        if (!is_inside) {
            state = Normal;
            is_focused = false;
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
            is_focused = false;
        }
        else if (!is_down) {
            is_focused = false;
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

// == CHECKBOX ==================================================================================

CheckBox::CheckBox(vec2i pos, bool default, int new_id, CallBack cb, void *udata) {
    atlas = Atlas("assets/GUI/checkbox.png", pos, { 2, Count });

    is_pressed = default;

    id = new_id;
    userdata = udata;
    callback = cb;
}

void CheckBox::update() {
    if (disabled) return;
    
    bool is_down = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    bool is_inside = atlas.getBox().contains(sys::getMousePos());

    if (!was_down && is_down) {
        was_inside_when_pressed = is_inside;
    }

    switch (state) {
    case Normal:
        if (is_inside && !is_focused) {
            state = Hover;
            is_focused = true;
        }
        break;
    case Hover:
        if (!is_inside) {
            state = Normal;
            is_focused = false;
        }
        else if (is_down && was_inside_when_pressed) {
            state = Pressed;
        }
        break;
    case Pressed:
        if (!is_inside) {
            state = Normal;
            is_focused = false;
        }
        else if (!is_down) {
            if (was_down) {
                is_pressed = !is_pressed;
                if (callback) {
                    callback(userdata, id, is_pressed);
                }
            }
            state = Hover;
            is_focused = false;
        }
        break;
    }

    was_down = is_down;

    Node::update();
}

void CheckBox::draw() {
    Node::draw();

    atlas.drawTile(is_pressed, state);
}

// == CHECKBOX ==================================================================================

Slider::Slider(vec2i new_pos, int new_width, float initial_value, int new_id, CallBack cb, void *udata) {
    atlas = Atlas("assets/GUI/slider.png", new_pos, { Count, 1 });

    atlas.pos -= atlas.getBox().size / 2;

    pos = new_pos;
    width = new_width;
    value = initial_value;

    id = new_id;
    userdata = udata;
    callback = cb;
}

void Slider::update() {
    if (disabled) return;
    
    recti box = atlas.getBox();
    bool is_down = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    bool is_inside = box.contains(sys::getMousePos());

    if (!was_down && is_down) {
        was_inside_when_pressed = is_inside;
    }

    switch (state) {
    case Normal:
        if (is_inside && !is_focused) {
            state = Hover;
            is_focused = true;
        }
        break;
    case Hover:
        if (!is_inside) {
            state = Normal;
            is_focused = false;
        }
        else if (is_down && was_inside_when_pressed) {
            state = Pressed;
        }
        break;
    case Pressed:
        if (!is_down) {
            state = is_inside ? Hover : Normal;
            is_focused = false;
        }
        else {
            // is_enabled = true;
            vec2i new_pos = sys::getMousePos();
            new_pos.x = clamp(new_pos.x, pos.x, pos.x + width);
            atlas.pos.x = new_pos.x - box.w / 2;
            value = (new_pos.x - pos.x) / (float)width;
            if (callback) {
                callback(userdata, id, value);
            }
        }
        break;
    }

    was_down = is_down;

    Node::update();
}

void Slider::draw() {
    Node::draw();

    DrawRectangle(pos.x, pos.y, width, 1, BLACK);
    atlas.drawTile(state);
}
