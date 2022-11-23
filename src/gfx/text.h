#pragma once

#include <string>

#include <raylib.h>
#include <node.h>
#include <maths.h>

struct Text : public Node {
    using Node::Node;
    
    Text() = default;
    Text(const char *text, vec2i pos = vec2i(), Color color = WHITE, int size = 8);

    vec2 getSize();

    void draw() override;

    vec2i pos;
    int size = 8;
    std::string text;
    Color color = WHITE;
};