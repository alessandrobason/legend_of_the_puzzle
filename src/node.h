#pragma once

#include <vector>

struct Node {
    Node() = default;
    Node(bool disabled) : disabled(disabled) {}

    virtual void init() {
        for (auto n : subnodes) {
            n->init();
        }
    }

    virtual void cleanup() {
        for (auto n : subnodes) {
            n->cleanup();
        }
    }

    virtual void update() {
        if (disabled) return;

        for (auto n : subnodes) {
            n->update();
        }
    }

    virtual void draw() {
        if (disabled) return;

        for (auto n : subnodes) {
            n->draw();
        }
    }

    void addSubnode(Node *node) {
        subnodes.emplace_back(node);
    }

    bool disabled = false;
    std::vector<Node *> subnodes;
};