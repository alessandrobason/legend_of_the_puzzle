#pragma once

extern "C" float GetFrameTime();

struct Timer {
    Timer() = default;
    Timer(float duration) : duration(duration) {}

    float duration = 1.f;
    float timer = 0.f;

    inline bool update() {
        timer += GetFrameTime();
        bool finished = timer >= duration;
        while (timer >= duration) {
            timer -= duration;
        }
        return finished;
    }

    inline float normalised() {
        return timer / duration;
    }

    inline void reset() {
        timer = 0.f;
    }
};