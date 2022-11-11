#pragma once
// #include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include <unordered_map>
#include <raylib.h>
#include "sprite.h"
#include "../maths.h"

class GameObject;

struct AnimatedSprite {
	typedef bool (GameObject::*Callback)(int anim_id);

	struct Animation {
		float duration = 0.2f;
		bool looping = true;
		std::vector<int> frames;
		Animation() = default;
		Animation(float d, bool l, const std::vector<int> &f) 
			: duration(d), looping(l), frames(f) {}
	};

	Sprite sprite;
	int columns;
	bool playing = true;
	float timer = 0.f;
	int cur_frame = 0;
	int cur_anim = -1;
	std::unordered_map<std::string, int> anim_map;
	std::vector<Animation> animations;

	GameObject *cb_obj = nullptr;
	Callback cb_fun = nullptr;

	AnimatedSprite();
	void init(Texture tex, int columns);
	void update(float dt);
	void draw();
	void reset();

	void updateSprite();

	void play(const std::string &name);
	void play(int id);
	const std::string &getAnimationName() const;
	int getAnimationId() const;
	int addAnimation(const std::string &name, std::vector<int> &&frames, float duration, bool looping = true);

	void setCallback(GameObject *object);
};

struct AnimatedTile {
	inline void update(float dt) {
		timer += dt;
		while (timer >= duration) {
			timer -= duration;
			cur_frame++;
			if (cur_frame >= frames.size()) {
				cur_frame = 0;
			}
		}
	}

	int getFrame() const {
		return frames[cur_frame];
	}

	int tile_id = -1;
	std::vector<int> frames;
	int cur_frame = 0;
	float timer = 0.f;
	float duration = 0.2f;
};
