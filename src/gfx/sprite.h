#pragma once

#include <raylib.h>
#include <maths.h>
#include <node.h>

struct Sprite : public Node {
    using Node::Node;
    
    Sprite() = default;
    Sprite(const char *filename);

    void draw() override;

    Texture texture;
    recti src;
    recti dst;
    vec2 origin;
    float angle = 0.f;
    Color color = WHITE;
};