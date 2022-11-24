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

struct Atlas : public Node {
    using Node::Node;

    Atlas() = default;
    Atlas(const char *filename, vec2i count = { 1, 1 }) 
        : Atlas(filename, { 0, 0 }, count) {}
    Atlas(const char *filename, vec2i pos, vec2i count = { 1, 1 });

    void draw() override;
    void drawTile(int x, int y = 0);

    recti getBox();

    Texture texture;
    vec2i count;
    vec2i pos;
    vec2 origin;
    float angle = 0.f;
    Color color = WHITE;
};