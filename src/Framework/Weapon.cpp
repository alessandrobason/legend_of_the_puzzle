#include "Weapon.h"

Weapon::Weapon(sf::Texture* txt, vec2i tex_coords, InputHandler* i, Collision::LAYER l, RoomManager* rm) {
	in = i;
	setTexture(*txt);
	setTextureRect(recti(tex_coords.x, tex_coords.y, 16, 16));
	setOrigin(12, 12);
	projectiles = Projectiles(txt, vec2i(16, 0), l, rm);
	projectiles.setOrientation(vec2(-1, -1));
}

void Weapon::handleInput(float dt) {
	if (in->isMouseLPressed()) {
		shoot();
	}
	angle_to_mouse -= 45;
}

void Weapon::update(float dt) {
	projectiles.update(dt);
	setRotation(angle_to_mouse);
}

void Weapon::drawArrows(sf::RenderWindow* w) {
	projectiles.draw(w);
}

void Weapon::drawDebug(sf::RenderWindow* w) {
	projectiles.drawDebug(w);
}

void Weapon::shoot() {
	projectiles.shoot(getTransform());
}

void Weapon::move(vec2 offset) {
	sf::Sprite::move(offset);
}