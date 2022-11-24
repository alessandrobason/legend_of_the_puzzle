#include "options_screen.h"

#include <system.h>
// #include <gfx/sprite.h>
#include <gfx/nineslice.h>
#include <gfx/text.h>
#include <gui.h>

#include <tracelog.h>

enum CheckBoxValues {
    None, FullScreen, Mute
};

void checkBoxCallBack(void *, int id, bool is_down) {
    switch (id) {
    case FullScreen: ToggleFullscreen();   break;
    case Mute:       SetMasterVolume(0.f); break;
    }
}

void OptionsScreen::init() {
    addSubnode(new NineSlice("assets/GUI/boxWithBackground.png", recti(vec2i(0, 0), sys::resolution)));
    addSubnode(new Text("Set fu llscreen", { 36, 32 }));
    addSubnode(new Text("Mute volume",    { 44, 51 }));
    addSubnode(new Text("Volume",         { 31, 70 }));
    addSubnode(new CheckBox({ 116, 33 }, false, FullScreen, checkBoxCallBack));
    addSubnode(new CheckBox({ 110, 52 }, false, Mute, checkBoxCallBack));
    addSubnode(new Slider({ 79, 74 }, 50, 0.5f, 0, [](void *, int, float volume) { info("v: %.3f", volume); SetMasterVolume(volume); }));
    addSubnode(new Button("go back", { 48, 89 }, { 64, 24 }, 0, [](void *, int) { sys::startTransition(sys::Main); }));

    Node::init();
}