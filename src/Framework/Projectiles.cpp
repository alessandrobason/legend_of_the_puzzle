#include "Projectiles.h"
#include "RoomManager.h"

Projectiles::Projectiles(Texture txt, vec2i tex_coords, Collision::LAYER l, RoomManager* rm) {
	texture = txt;
	texture_coordinates = (vec2)tex_coords;
	collisionlayer = l;
	roommanager = rm;
	projectilesize = vec2(16, 16);
}

void Projectiles::update(float dt) {
	for (size_t i = 0; i < projectile_vector.size(); i++) {
		if (projectile_vector[i].hitsomething) continue;
		rect mapboundaries;
		mapboundaries.x = roommanager->getCurrentRoom()->getBound(RoomManager::LEFT).x;
		mapboundaries.y = roommanager->getCurrentRoom()->getBound(RoomManager::TOP).y;
		mapboundaries.w = roommanager->MAPSIZE;
		mapboundaries.h = roommanager->MAPSIZE;
		if (!projectile_vector[i].collider.Check_Collision(mapboundaries)) {
			removeArrow(i);
			i--;
			continue;
		}
		sf::Vertex* v = &vertexs[i * 4];
		vec2 dt_velocity = projectile_vector[i].velocity * dt;
		v[0].position += dt_velocity;
		v[1].position += dt_velocity;
		v[2].position += dt_velocity;
		v[3].position += dt_velocity;
		projectile_vector[i].collider.moveCollision(dt_velocity);

		Collision* collision_hit = nullptr;

		// check game objects collisions
		for (size_t j = 0; j < roommanager->getCurrentRoom()->getGameObjects()->size(); j++) {
			Collision* current_collision = &roommanager->getCurrentRoom()->getGameObjects()->at(j)->collider;
			if (collisionlayer == current_collision->collisionlayer) continue;
			if (current_collision->collisionlayer == Collision::LAYER::NONE) continue;
			rect rect = current_collision->rect;
			if (projectile_vector[i].collider.Check_Collision(rect)) {
				collision_hit = current_collision;
				projectile_vector[i].hitsomething = true;
				projectilestween.push_back(Tweening<float>(255.f, 0.f, 0.f));
				removing_projectiles.push_back({ i });
				roommanager->getCurrentRoom()->getGameObjects()->at(j)->hit(damage);
				break;
			}
		}
		// check tilemap collisions
		for (size_t j = 0; j < roommanager->getCurrentRoom()->getColliders()->size(); j++) {
			Collision* current_collision = roommanager->getCurrentRoom()->getColliders()->at(j);
			if (collisionlayer == current_collision->collisionlayer) continue;
			if (current_collision->collisionlayer == Collision::LAYER::WATER) continue;
			rect rect = current_collision->rect;
			if (projectile_vector[i].collider.Check_Collision(rect)) {
				collision_hit = current_collision;
				projectile_vector[i].hitsomething = true;
				projectilestween.push_back(Tweening<float>(255.f, 0.f, 1.f));
				removing_projectiles.push_back({ i });
				break;
			}
		}
	}

	for (size_t i = 0; i < removing_projectiles.size(); i++) {
		projectilestween[i].update(dt);
		size_t k = removing_projectiles[i].positioninarray * 4;
		for (size_t j = 0; j < 4; j++) {
			vertexs[k + j].color = sf::Color(255, 255, 255, projectilestween[i].getValue());
		}
		if (projectilestween[i].isfinished()) {
			removeArrow(removing_projectiles[i].positioninarray);
			i--;
		}
	}
}

// void Projectiles::shoot(sf::Transform transform) {
void Projectiles::shoot(vec2 position, vec2 direction) {
	singleprojectile newprojectile;
	// sf::Vertex vertex[4];

	// transform.translate(vec2(-projectilehitbox.w / 2, -projectilehitbox.h / 2));

	// newprojectile.position = vec2(transform.getMatrix()[12], transform.getMatrix()[13]);
	newprojectile.position = position;
	newprojectile.velocity = direction * speed;
	// newprojectile.velocity = transform.transformPoint(orientation * speed);
	newprojectile.velocity -= position;
	vec2 centre = projectilehitbox.centre();
	// vec2 centerofcollision = transform.transformPoint(center);
	newprojectile.collider = { rect(centre - projectilehitbox.size / 2.f), collisionlayer };
	// vec2 centerofcollision = centre;
	// newprojectile.collider = Collision( centerofcollision.x - projectilehitbox.w / 2,
	// 									centerofcollision.y - projectilehitbox.h / 2,
	// 									projectilehitbox.w,
	// 									projectilehitbox.h,
	// 									collisionlayer);
	projectile_vector.push_back(newprojectile);

	// vertex[0].position = transform.transformPoint(vec2());
	// vertex[1].position = transform.transformPoint(vec2(projectilesize.x, 0));
	// vertex[2].position = transform.transformPoint(vec2(projectilesize.x, projectilesize.y));
	// vertex[3].position = transform.transformPoint(vec2(0, projectilesize.y));

	// vertex[0].texCoords = texture_coordinates;
	// vertex[1].texCoords = texture_coordinates + vec2(projectilesize.x, 0);
	// vertex[2].texCoords = texture_coordinates + vec2(projectilesize.x, projectilesize.y);
	// vertex[3].texCoords = texture_coordinates + vec2(0, projectilesize.y);

	// for (size_t i = 0; i < 4; i++) vertexs.push_back(vertex[i]);
}

void Projectiles::removeArrow(size_t n) {
	size_t vn = n * 4;
	vertexs.erase(vertexs.begin() + vn, vertexs.begin() + vn + 4);
	projectile_vector.erase(projectile_vector.begin() + n);
	
	for (size_t i = 0; i < removing_projectiles.size(); i++) {
		if (removing_projectiles[i].positioninarray == n) {
			projectilestween.erase(projectilestween.begin() + i);
			removing_projectiles.erase(removing_projectiles.begin() + i);
			i--;
			continue;
		}
		if (removing_projectiles[i].positioninarray > n) removing_projectiles[i].positioninarray--;
	}
	
}

void Projectiles::draw() {
	
}

void Projectiles::drawDebug() {
	for (auto &proj : projectile_vector) {
		proj.collider.drawDebug();
	}
}