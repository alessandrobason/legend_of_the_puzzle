#include "Player.h"
#include "Framework/RoomManager.h"

Player::Player() : GameObject(nullptr, nullptr, nullptr){}

Player::Player(InputHandler* input, RoomManager* rm, sf::RenderWindow* win) : GameObject(input, rm, win){
    collisionlayer = Collision::LAYER::PLAYER;

    // load player data from json
    config = new JSONparser("Data/player_data.json");

    playerdata.baselife = (float)config->doc["life"].d;

    setSpeed((float)config->doc["speed"].i);
    setLife(playerdata.baselife);

    auto &json_collision = config->doc["collision"].obj;
    float x = (float)json_collision["x"].i;
    float y = (float)json_collision["y"].i;
    float w = (float)json_collision["width"].i;
    float h = (float)json_collision["height"].i;
    collider = Collision(x, y, w, h, collisionlayer);

    auto &json_spritesheet = config->doc["spritesheet"].obj;
    int rows = json_spritesheet["rows"].i;
    int columns = json_spritesheet["columns"].i;

    if (!roommanager->textures["player"].loadFromFile(config->doc["spritesheet"].obj["image"].str)) {
        std::cout << "Failed to load " << config->doc["spritesheet"].str << "\n";
    }

    // animated sprite
    animSprite.setSpriteSheet(&roommanager->textures["player"], columns, rows);

    auto &json_animations = config->doc["animations"].arr;
    for (auto &anim : json_animations) {
        std::vector<int> animFrames;
        float speed = (float)anim.obj["speed"].d;
        bool looping = anim.obj["looping"].b;
        for (auto &frame : anim.obj["array"].arr) {
            animFrames.emplace_back(frame.i);
        }
        auto &anim_name = anim.obj["name"].str;
        animSprite.addAnimation(anim_name, std::move(animFrames), speed, looping);
    }
    animSprite.setCurrentAnimation("idle up-left");

    local_center = collider.collision_offset + vec2(collider.rect.width / 2, collider.rect.height / 2);
    
    // LOAD WEAPONs
    if (!roommanager->textures["weapons"].loadFromFile(config->doc["weapons"].obj["spritesheet"].str)) {
        std::cout << "Failed to load weapons texture from " << config->doc["weapons"].obj["spritesheet"].str << "\n";
    }

    vec2i bow_texture_coordinates;
    bow_texture_coordinates.x = config->doc["weapons"].obj["bow"].obj["texture coordinates"].arr[0].i;
    bow_texture_coordinates.y = config->doc["weapons"].obj["bow"].obj["texture coordinates"].arr[1].i;
    bow = Weapon(&roommanager->textures["weapons"], bow_texture_coordinates, in, collisionlayer, roommanager);

    rectf arrowhitbox;
    arrowhitbox.top = (float)config->doc["weapons"].obj["arrow"].obj["hitbox"].arr[0].i;
    arrowhitbox.left = (float)config->doc["weapons"].obj["arrow"].obj["hitbox"].arr[1].i;
    arrowhitbox.width = (float)config->doc["weapons"].obj["arrow"].obj["hitbox"].arr[2].i;
    arrowhitbox.height = (float)config->doc["weapons"].obj["arrow"].obj["hitbox"].arr[3].i;
    bow.setHitBox(arrowhitbox);
    playerdata.basedamage = (float)config->doc["weapons"].obj["bow"].obj["base_damage"].d;
    bow.setDamage(playerdata.basedamage);

    // CHECKBOX
    checkboxsize = vec2(300, 300);
    checkbox = Collision(-checkboxsize.x / 2.f, -checkboxsize.y / 2.f, checkboxsize.x / 2.f, checkboxsize.y / 2.f, Collision::LAYER::PLAYER);
    checkbox.setDebugColor(sf::Color::Yellow);

    // PLAYER GUI
    playerdata.expneeded = nextLevelExp(playerdata.level);
    if (!roommanager->textures["playerbar"].loadFromFile("Assets/lifeexpbar.png")) {
        std::cout << "Couldn't load player bar texture\n";
        abort();
    }
    roommanager->getPlayerGui()->setBarTexture(&roommanager->textures["playerbar"]);
    roommanager->getPlayerGui()->setTotalLife(playerdata.totallife);
    roommanager->getPlayerGui()->setNextLevelExp(playerdata.expneeded);
}

void Player::move(vec2 offset) {
    GameObject::move(offset);
    bow.move(offset);
    checkbox.moveCollision(offset);
}

void Player::setPosition(vec2 pos) {
    getSprite()->setPosition(pos);
    collider.setPosition(vec2(pos.x + collider.collision_offset.x, pos.y + collider.collision_offset.y));
    bow.setPosition(pos + local_center);
    checkbox.setCenter(pos + local_center);
}

void Player::hit(float damage) {
    if (flashing.isflashing) return;
    if (getCheat(CHEATS::GODMODE)) return;

    roommanager->playHitSound();
    playerdata.life -= damage;
    if (playerdata.life <= 0) {
        roommanager->setCurrentState(RoomManager::STATES::GAMEOVER);
        canmove = false;
        animSprite.setCurrentAnimation("death");
        return;
    }
    flashing.isflashing = true;

    roommanager->getPlayerGui()->updateLifeValue(playerdata.life);
}

void Player::giveExp(float e) {
    std::cout << "exp given: " << e << " current exp: " << playerdata.exp << " needed: " << playerdata.expneeded << "\n";
    playerdata.exp += e;
    if (playerdata.exp >= playerdata.expneeded) {
        roommanager->playSound("level-up");

        float remain = playerdata.exp - playerdata.expneeded;
        roommanager->getPlayerGui()->updateExpValue(playerdata.expneeded);
        playerdata.exp = remain;
        roommanager->getPlayerGui()->remainExpValue(remain);

        levelUp();
    }
    else {
        roommanager->getPlayerGui()->updateExpValue(playerdata.exp);
    }
}

void Player::levelUp() {
    playerdata.level++;
    playerdata.expneeded = nextLevelExp(playerdata.level);
    roommanager->getPlayerGui()->setNextLevelExp(playerdata.expneeded);

    playerdata.totallife += playerdata.baselife;
    playerdata.life = playerdata.totallife;
    roommanager->getPlayerGui()->setTotalLife(playerdata.totallife);
    roommanager->getPlayerGui()->updateLifeValue(playerdata.life);
    
    bow.setDamage(bow.getDamage() + 2.5f * playerdata.level);
}

float Player::nextLevelExp(int level) {
    return level * 6.f;
}

void Player::reset() {
    playerdata.level = 1;
    playerdata.exp = 0.f;
    playerdata.totallife = playerdata.baselife;
    playerdata.life = playerdata.baselife;
    bow.setDamage(playerdata.basedamage);
    playerdata.expneeded = nextLevelExp(playerdata.level);
    roommanager->getPlayerGui()->updateExpValue(playerdata.exp);
    roommanager->getPlayerGui()->setNextLevelExp(playerdata.expneeded);
    getAnimation()->setCurrentAnimation("idle up-left");
}

void Player::handleInput(float dt) {
    oldVel = vel;
    if (canmove) {
        vel = vec2(0, 0);

        vel.x = (float)(in->isKeyDown(in->KEY_RIGHT) - in->isKeyDown(in->KEY_LEFT));
        vel.y = (float)(in->isKeyDown(in->KEY_DOWN) - in->isKeyDown(in->KEY_UP));

        vec2 mousepos = in->getMouseRelative();
        vec2 center = getSprite()->getPosition() + local_center;

        center.x = mousepos.x - center.x;
        center.y = mousepos.y - center.y;
        angle_to_mouse = UsefulFunc::atan2(center.y, center.x) * 180 / (float)M_PI + 180;
        bow.setAngle(angle_to_mouse);

        switch ((int)(angle_to_mouse / 45)) {
        case 0:
        case 1:
            last_direction = UP_LEFT;
            draw_weapon_over = false;
            break;
        case 2:
        case 3:
            last_direction = UP_RIGHT;
            draw_weapon_over = false;
            break;
        case 4:
        case 5:
            last_direction = DOWN_RIGHT;
            draw_weapon_over = true;
            break;
        case 6:
        case 7:
            last_direction = DOWN_LEFT;
            draw_weapon_over = true;
            break;
        }

        vel = UsefulFunc::normalize(vel) * speed;

        std::string anim = "idle ";
        bool ismoving = vel != vec2(0, 0);
        if (ismoving) anim = "walk ";

        anim += directionnames[last_direction];

        if (in->isKeyPressed(in->KEY_MENU)) {
            // roommanager->textures["pausescreenshot"].create(w->getSize());
            // roommanager->textures["pausescreenshot"].update(*w);
            roommanager->textures["pausescreenshot"].takeScreenshot();

            roommanager->moveMenu("pause");
        }

        if (in->isMouseRPressed() && ismoving) {
            canmove = false;
            isdodging = true;
            anim = "dodge ";

            if (vel.y < 0) anim += "up-";
            else           anim += "down-";

            if (vel.x < 0) anim += "left";
            else           anim += "right";

            collider.collisionlayer = Collision::LAYER::NONE;
        }

        animSprite.setCurrentAnimation(anim);
        bow.handleInput(dt);
    }
    else if (isdodging) {
        if (!animSprite.getPlaying()) {
            canmove = true;
            isdodging = false;
            collider.collisionlayer = Collision::LAYER::PLAYER;
        }
    }
}

bool Player::animationCallback(std::string name) {
    if (name == "death") {
        roommanager->moveMenu("deathscreen");
        return true;
    }

    return false;
}

void Player::update(float dt) {
    animSprite.animate(dt);
    bow.update(dt);
    if (canmove || isdodging) move(vel * dt);
    if (flashing.isflashing) flash(dt);
}

void Player::updateAnimation(float dt) {
    std::string anim = "idle ";
    if (vel != vec2()) anim = "walk ";

    if (vel.y < 0) anim += "up-";
    else           anim += "down-";
    if (vel.x < 0) anim += "left";
    else           anim += "right";

    std::cout << "anim: " << anim << "\n";

    animSprite.setCurrentAnimation(anim);
    animSprite.animate(dt);
}

void Player::draw() {
    if (!draw_weapon_over && !isdodging) w->draw(bow);
    animSprite.draw(w);
    if (draw_weapon_over && !isdodging) w->draw(bow);
    bow.drawArrows(w);
}

void Player::drawDebug(){
    GameObject::drawDebug();
    bow.drawDebug(w);
    checkbox.drawDebug(w);
}

void Player::setVisible(bool v) {
    GameObject::setVisible(v);
    bow.setColor(flashing.color[v]);
}