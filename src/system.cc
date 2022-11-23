#include "system.h"

#include <string.h>
#include <unordered_map>

#include "tracelog.h"
#include "timer.h"
#include "tween.h"

// menus
#include "screens/splash_screen.h"
#include "screens/start_screen.h"
#include "screens/main_menu.h"
#include "screens/map_screen.h"
#include "screens/pause_menu.h"
#include "screens/debug_menu.h"
#include "screens/credits_screen.h"
#include "screens/options_screen.h"
#include "screens/controls_screen.h"
#include "screens/leaderboard_screen.h"

struct Root : public Node {
    using Node::Node;
    
    void update() override {
        Node::update();
        updateTransition();
    }

    void draw() override {
        Node::draw();
        drawTransition();
    }

    void setScreen(sys::Screen screen) {
        if (tran_screen != sys::None) {
            err("already transitioning screen!");
            return;
        }

        if (cur_screen == sys::None) {
            cur_screen = screen;
            subnodes[cur_screen]->disabled = false;
            return;
        }

        tran_screen = screen;
        tran_state = TranState::Starting;
        tran_timer.reset();
    }

    void updateTransition() {
        if (tran_state == TranState::None) {
            return;
        }

        if (tran_timer.update()) {
            switch (tran_state) {
            case TranState::Starting: 
                tran_state = TranState::Finishing; 
                subnodes[cur_screen]->disabled = true;
                subnodes[tran_screen]->disabled = false;
                cur_screen = tran_screen;
                tran_screen = sys::None;
                break;
            case TranState::Finishing: 
                tran_state = TranState::None; 
                break;
            }
        }
    }

    void drawTransition() {
        if (tran_state == TranState::None) {
            return;
        }

        vec2 res = sys::resolution;
        float start = 0.f, end = 0.f;

        switch (tran_state) {
        case TranState::Starting:  end = res.y / 2.f; break;
        case TranState::Finishing: start = res.y / 2.f; break;
        }

        float size = lerp(start, end, tran_timer.normalised());
        DrawRectangle(0, 0, (int)res.x, (int)size, BLACK);
        DrawRectangle(0, (int)(res.y - size), (int)res.x, (int)size + 1, BLACK);
    }

    enum class TranState {
        None, Starting, Finishing
    };

    sys::Screen cur_screen = sys::None;
    sys::Screen tran_screen = sys::None;
    TranState tran_state = TranState::None;
    Timer tran_timer = 0.5f;
};

namespace sys {
    static Root root;
    static bool is_running = true;
    static std::unordered_map<std::string, Texture> texture_cache;
    Camera2D camera;
    vec2i resolution;
    Font font;

    Node &getRoot() {
        return root;
    }

    void init() {
        SetExitKey(KEY_NULL);

        font = LoadFont("assets/zelda2.ttf");

        root.addSubnode(new Node(true)); // empty node for Scene::None
        root.addSubnode(new SplashScreen(true));
        root.addSubnode(new StartScreen(true));
        root.addSubnode(new MainMenu(true));
        root.addSubnode(new MapScreen(true));
        root.addSubnode(new PauseMenu(true));
        root.addSubnode(new DebugMenu(true));
        root.addSubnode(new CreditsScreen(true));
        root.addSubnode(new OptionsScreen(true));
        root.addSubnode(new ControlsScreen(true));
        root.addSubnode(new LeaderBoardScreen(true));

        root.setScreen(Screen::Controls);
        
        root.init();
    }

    void update() {
        root.update();
    }

    void draw() {
        root.draw();
    }

    void cleanup() {
        root.cleanup();
    }

    vec2 getMousePos() {
        return GetScreenToWorld2D(GetMousePosition(), camera);
    }

    bool isRunning() {
        return is_running && !WindowShouldClose();
    }

    void closeWindow() {
        is_running = false;
    }

    void startTransition(Screen screen) {
        root.setScreen(screen);
    }

    Texture loadTexture(const char *filename) {
        if (!filename) {
            return Texture{0};
        }
        
        auto it = texture_cache.find(filename);
        if (it == texture_cache.end()) {
            Texture t = LoadTexture(filename);
            texture_cache[filename] = t;
            return t;
        }
        return it->second;
    }

} // namespace sys
