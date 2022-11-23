#pragma once

#include <raylib.h>
#include <maths.h>
#include <node.h>

struct NineSlice : public Node {
    using Node::Node;
    
    NineSlice() = default;
    NineSlice(const char *filename);
    NineSlice(const char *filename, const recti &dst);

    void draw() override;

    Texture texture;
    recti dst;
};