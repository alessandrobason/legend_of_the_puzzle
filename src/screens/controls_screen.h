#pragma once

#include <node.h>

struct ControlsScreen : public Node {
    using Node::Node;
    
    virtual void init() override;
    // virtual void cleanup() override {}
    // virtual void update() override {}
    // virtual void draw() override {}
};