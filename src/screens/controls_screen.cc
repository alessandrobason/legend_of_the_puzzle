#include "controls_screen.h"

#include <system.h>
#include <gfx/sprite.h>
#include <gfx/nineslice.h>
#include <gfx/text.h>
#include <gui.h>

void ControlsScreen::init() {
    addSubnode(new NineSlice("assets/GUI/boxWithBackground.png", recti(vec2i(0, 0), sys::resolution)));
    addSubnode(new Sprite("assets/GUI/controls.png"));
    addSubnode(new Text("pause",    { 26, 54 }));
    addSubnode(new Text("move",     { 86, 54 }));
    addSubnode(new Text("interact", { 76, 67 }));
    addSubnode(new Text("shoot",    { 10, 82 }));
    addSubnode(new Text("dodge",    { 41, 82 }));
    addSubnode(new Text("debug",    { 84, 82 }));
    addSubnode(new Button("back", { 118, 114 }, { 32, 20 }, 0, [](void *, int) { sys::startTransition(sys::Main); }));

    Node::init();
}