#pragma once
// #include <SFML/Graphics.hpp>
#include <vector>
#include "../GameObjects/Enemy.h"
#include "RoomManager.fwd.h"
#include "Tweening.h"

class Projectiles {
	struct singleprojectile {
		vec2 position;
		vec2 velocity;
		Collision collider;
		bool hitsomething = false;
	};

	struct projectiledeath {
		size_t positioninarray;
		float alpha = 255.f;
	};

	vec2 projectilesize;
	rect projectilehitbox;
	vec2 orientation = vec2(0, 0);

	float speed = 100.f;
	float damage = 0.f;
	vec2 texture_coordinates;
	std::vector<sf::Vertex> vertexs;
	//sf::VertexArray vertexs;
	sf::RenderStates states;
	// stores all the data of projectiles, its size
	// should be equal to projectils_vertexs' size / 4
	std::vector<singleprojectile> projectile_vector;
	std::vector<projectiledeath> removing_projectiles;

	Collision::LAYER collisionlayer = Collision::LAYER::ALL;

	RoomManager* roommanager = nullptr;
	std::vector<Collision*>* tilemapcollisions = nullptr;
	std::vector<GameObject*>* gameobjects = nullptr;

	std::vector<Tweening<float>> projectilestween;
public:
	Projectiles() {}

	Projectiles(sf::Texture* txt, vec2i tex_coords, Collision::LAYER l, RoomManager* rm);

	void setGameObjects(std::vector<GameObject*>* g) { gameobjects = g; }
	void setTilemap(std::vector<Collision*>* t) { tilemapcollisions = t; }
	void setHitBox(rect hb) { projectilehitbox = hb; }
	void setSize(vec2 s) { projectilesize = s; }
	void setDamage(float d) { damage = d; }
	float getDamage() { return damage; }
	void setOrientation(vec2 o) { orientation = o; }

	void update(float dt);

	void draw(sf::RenderWindow* w) {
		w->draw(vertexs.data(), vertexs.size(), sf::Quads, states);
	}

	void drawDebug(sf::RenderWindow* w) {
		for (size_t i = 0; i < projectile_vector.size(); i++) {
			projectile_vector[i].collider.drawDebug(w);
		}
	}

	void shoot(sf::Transform transform);
	void removeArrow(size_t n);

	void setSpeed(float s) { speed = s; }
};