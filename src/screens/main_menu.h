#pragma once

#include <raylib.h>

#include <node.h>
#include <gui.h>

struct MainMenu : public Node {
    using Node::Node;
    virtual void init() override;
};