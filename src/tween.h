#pragma once

#include "timer.h"

template<typename T>
T lerp(const T &beg, const T &end, float t) {
    return beg * (1 - t) + end * t;
}

template<typename T>
struct Tween {
    T start;
    T end;
    Timer time;
    bool finished = false;

    T update() {
        finished = time.update();
        return lerp(start, end, time.normalised());
    }
};