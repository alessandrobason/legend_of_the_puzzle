#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include <unordered_map>

class GameObject;

struct AnimatedSprite {
	typedef bool (GameObject::*Callback)(std::string name);

	struct Animation {
		float duration = 0.2f;
		bool looping = true;
		std::vector<int> frames;
		Animation() = default;
		Animation(float d, bool l, const std::vector<int> &f) 
			: duration(d), looping(l), frames(f) {}
	};

	sf::Sprite sprite;
	int columns;
	bool playing = true;
	float timer = 0.f;
	int cur_frame = 0;
	int cur_anim = -1;
	std::string cur_anim_name;
	std::unordered_map<std::string, int> anim_map;
	std::vector<Animation> animations;

	// GameObject *cb_obj = nullptr;
	// Callback cb_fun = nullptr;
	std::function<void(const std::string &)> cb_fun;

	AnimatedSprite();
	void setSpriteSheet(sf::Texture *tex, int columns, int rows);
	void animate(float dt);
	void draw(sf::RenderWindow *w);
	void reset();

	void updateSprite();

	void setCurrentAnimation(const std::string &name);
	void setCurrentAnimation(int id);
	const std::string &getCurrentAnimation() const;
	int getAnimationId() const;
	int addAnimation(const std::string &name, std::vector<int> &&frames, float duration, bool looping = true);

	void setCallbackObject(GameObject *object);

	bool getPlaying() const { return playing; }

private:
	void playInternal(int id, const std::string &name = {});
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