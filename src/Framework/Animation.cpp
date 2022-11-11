#include "Animation.h"
#include "GameObject.h"

#include <assert.h>

AnimatedSprite::AnimatedSprite() {
	// cb_fun = &GameObject::animationCallback;
}

void AnimatedSprite::init(Texture tex, int new_columns) {
	sprite.tex = tex;
	sprite.tex_rect.size = { (float)tex.width, (float)tex.height };
	columns = new_columns;
}

void AnimatedSprite::update(float dt) {
	if (!playing || cur_anim < 0 || cur_anim >= animations.size()) {
		return;
	}

	Animation &anim = animations[cur_anim];
	timer += dt;
	if (timer >= anim.duration) {
		timer = 0;
		cur_frame++;
		if (cur_frame >= anim.frames.size()) {
			if (cb_obj) {
				// (cb_obj->*cb_fun)(cur_anim);
			}
			if (anim.looping) {
				cur_frame = 0;
			}
			else {
				cur_frame = (int)anim.frames.size() - 1;
				playing = false;
			}
		}
		updateSprite();
	}
}

void AnimatedSprite::draw() {
	sprite.draw();
}

void AnimatedSprite::updateSprite() {
	if (!playing || cur_anim < 0 || cur_anim >= animations.size()) {
		return;
	}

	int frame = animations[cur_anim].frames[cur_frame];
	sprite.tex_rect.pos = {
		(float)(frame % columns),
		(float)(frame / columns)
	};
	sprite.tex_rect.pos *= sprite.tex_rect.size;
}

void AnimatedSprite::reset() {
	cur_frame = 0;
	play(0);
	timer = 0.f;
	playing = true;
	updateSprite();
}

void AnimatedSprite::play(const std::string &name) {
	play(anim_map[name]);
}

void AnimatedSprite::play(int id) {
	if (id < 0 || id >= animations.size()) {
		return;
	}
	if (cur_anim != id) {
		cur_anim = id;
		reset();
	}
}

const std::string &AnimatedSprite::getAnimationName() const {
	for (const auto &anim : anim_map) {
		if (anim.second == cur_anim) {
			return anim.first;
		}
	}
	static std::string no_name{};
	return no_name;
}

int AnimatedSprite::getAnimationId() const {
	return cur_anim;
}

int AnimatedSprite::addAnimation(
	const std::string &name, 
	std::vector<int> &&frames, 
	float duration, 
	bool looping
) {
	int id = (int)animations.size();
	anim_map[name] = id;
	animations.emplace_back(duration, looping, std::move(frames));
	return id;
}

void AnimatedSprite::setCallback(GameObject *object) {
	cb_obj = object;
}