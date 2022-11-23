#include "Animation.h"
#include "GameObject.h"

AnimatedSprite::AnimatedSprite() {
	// cb_fun = &GameObject::animationCallback;
}

void AnimatedSprite::setSpriteSheet(sf::Texture *tex, int new_columns, int rows) {
	sprite.setTexture(*tex);
	sprite.setTextureRect({ vec2i(0, 0), tex->getSize() });
	columns = new_columns;
}

void AnimatedSprite::animate(float dt) {
	if (!playing || cur_anim < 0 || cur_anim >= animations.size()) {
		return;
	}

	Animation &anim = animations[cur_anim];
	timer += dt;
	if (timer >= anim.duration) {
		timer = 0;
		cur_frame++;
		if (cur_frame >= anim.frames.size()) {
			if (cb_fun) {
				cb_fun(cur_anim_name);
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

void AnimatedSprite::draw(sf::RenderWindow *w) {
	w->draw(sprite);
}

void AnimatedSprite::updateSprite() {
	if (!playing || cur_anim < 0 || cur_anim >= animations.size()) {
		return;
	}

	int frame = animations[cur_anim].frames[cur_frame];
	sprite.m_tex_rect.pos = {
		frame % columns,
		frame / columns
	};
	sprite.m_tex_rect.pos *= sprite.m_tex_rect.size;
}

void AnimatedSprite::reset() {
	cur_frame = 0;
	setCurrentAnimation(0);
	timer = 0.f;
	playing = true;
	updateSprite();
}

void AnimatedSprite::setCurrentAnimation(const std::string &name) {
	playInternal(anim_map[name], name);
}

void AnimatedSprite::setCurrentAnimation(int id) {
	playInternal(id);
}

void AnimatedSprite::playInternal(int id, const std::string &name) {
	if (id < 0 || id >= animations.size()) {
		return;
	}

	if (name.empty()) {
		for (const auto &anim : anim_map) {
			if (anim.second == cur_anim) {
				cur_anim_name = anim.first;
				break;
			}
		}
	}
	else {
		cur_anim_name = name;
	}

	if (cur_anim != id) {
		cur_anim = id;
		reset();
	}
}


const std::string &AnimatedSprite::getCurrentAnimation() const {
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

void AnimatedSprite::setCallbackObject(GameObject *object) {
	cb_fun = [&object] (const std::string &name) {
		object->animationCallback(name);
	};
}