#include "Octorock.h"
#include "../Map_room.h"
#include "../Framework/RoomManager.h"

Octorock::Octorock(const Enemy& e) : Enemy(e) {
	projectiles = Projectiles(&roommanager->textures["enemyproj"], vec2i(), Collision::LAYER::ENEMY, roommanager);
	projectiles.setHitBox(rectf(0, 0, 9, 8));
	projectiles.setOrientation(vec2(-1, 0));
	projectiles.setSize(vec2(9, 8));
	projectiles.setDamage(damage);
}

void Octorock::update(float dt) {
	Enemy::update(dt);
	projectiles.update(dt);
	if (timepassed.getElapsedTime().asSeconds() >= shootingspeed) {
		shooting = false;
	}
}

void Octorock::draw() {
	Enemy::draw();
	projectiles.draw(w);
}

void Octorock::drawDebug() {
	Enemy::drawDebug();
	projectiles.drawDebug(w);
}

void Octorock::hit(float damage) {
	if (flashing.isflashing) return;
	if (life <= 0) return;

	life -= damage;
	roommanager->playHitSound();

	if (life <= 0) {
		collider.disable();
		dead = true;
		animation.setCurrentAnimation("death");
		roommanager->getPlayer()->giveExp(5.f);
		return;
	}
	flashing.isflashing = true;
}

void Octorock::Advance() {
	if (dead) return;
	switch (currentstate) {
	case states::IDLE:
		if (isplayerinside) {
			currentstate = states::MOVING;
		}
		else if (target == vec2()) {
			int x = rand() % (int)checkbox.rect.width - (int)(checkbox.rect.width / 2);
			int y = rand() % (int)checkbox.rect.height - (int)(checkbox.rect.height / 2);
			target = vec2i(x, y) + getCenter();
		}
		break;
	case states::ATTACK:
		if (!roommanager->getPlayer()->checkbox.rect.intersects(collider.rect)) {
			currentstate = states::MOVING;
		}
		if (isplayerinside) {
			if (!shooting) {
				sf::Transform t;
				t.translate(getCenter());
				t.rotate(angletoplayer);
				projectiles.shoot(t);
				timepassed.restart();
				shooting = true;
			}
		}
		else {
			currentstate = states::IDLE;
		}
		break;
	case states::MOVING:
		target = roommanager->getPlayer()->getCenter();

		if (!isplayerinside) {
			target = vec2();
			currentstate = states::IDLE;
		}
		else if (roommanager->getPlayer()->checkbox.rect.intersects(collider.rect)) {
			target = vec2();

			currentstate = states::ATTACK;
		}
		break;
	default:
		break;
	}
}

bool Octorock::animationCallback(std::string name) {
	if (name == "death") {
		die();
		return true;
	}
	return false;
}