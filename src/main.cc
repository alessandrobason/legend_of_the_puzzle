#include <stdlib.h>
#include <time.h>

#include "tracelog.h"
#include "system.h"

int main() {
    srand((unsigned)time(NULL));

    sys::resolution = vec2i(160, 144);
    sys::camera.zoom = 5.f;
    
    vec2i size = sys::resolution * (int)sys::camera.zoom;
    InitWindow(size.x, size.y, "Legend of the Puzzle");
    SetTargetFPS(60);

    sys::init();

    while (sys::isRunning()) {
        sys::update();
 
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(sys::camera);
                sys::draw();
            EndMode2D();
        EndDrawing();
    }

    sys::cleanup();
}
