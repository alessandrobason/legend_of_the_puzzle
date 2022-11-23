#pragma once

#include "rl.h"
#include "maths.h"
#include "node.h"

struct Node;

namespace sys {
    enum Screen {
        None,
        Splash,
        Start,
        Main,
        Map,
        Pause,
        Debug,
        Credits,
        Options,
        Controls,
        LeaderBoard,
    };
    
    Node &getRoot();

    void init();
    void update();
    void draw();
    void cleanup();

    vec2 getMousePos();

    bool isRunning();
    void closeWindow();
    void startTransition(Screen screen);

    Texture loadTexture(const char *filename);

    extern Camera2D camera;
    extern vec2i resolution;
    extern Font font;
} // namespace sys