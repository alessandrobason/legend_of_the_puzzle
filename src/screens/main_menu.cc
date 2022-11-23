#include "main_menu.h"

#include <tracelog.h>
#include <system.h>

#include <gfx/sprite.h>

enum ButtonIds {
    None, Start, Options, Controls, Leaderboard, Exit
};

void buttonCallback(void *udata, int id) {
    switch (id) {
    case Start:       sys::startTransition(sys::Start);       break; 
    case Options:     sys::startTransition(sys::Options);     break; 
    case Controls:    sys::startTransition(sys::Controls);    break;
    case Leaderboard: sys::startTransition(sys::LeaderBoard); break;
    case Exit:        sys::closeWindow(); break;
    }
}

void MainMenu::init() {
    addSubnode(new Sprite("assets/GUI/mainmenu.png"));
    
    addSubnode(new Button("start",       vec2i(9, 12),  vec2i(72, 24), Start,       buttonCallback));
    addSubnode(new Button("options",     vec2i(9, 36),  vec2i(72, 24), Options,     buttonCallback));
    addSubnode(new Button("controls",    vec2i(9, 60),  vec2i(72, 24), Controls,    buttonCallback));
    addSubnode(new Button("leaderboard", vec2i(9, 84),  vec2i(72, 24), Leaderboard, buttonCallback));
    addSubnode(new Button("exit",        vec2i(9, 108), vec2i(72, 24), Exit,        buttonCallback));

    Node::init();
}
