#pragma once

#include <node.h>

struct OptionsScreen : public Node {
    using Node::Node;

    virtual void init() override;
};