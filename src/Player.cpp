#include "Player.h"
#include "Framework/RoomManager.h"

Player::Player() : GameObject(nullptr){}

Player::Player(RoomManager* rm) : GameObject(rm){
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

    // animated sprite
    Texture player_tex = roommanager->loadTexture("player", config->doc["spritesheet"].obj["image"].str.c_str());
    animSprite.init(player_tex, columns);

    auto &json_animations = config->doc["animations"].arr;
    for (auto &anim : json_animations) {
        std::vector<int> animFrames;
        float speed = (float)anim.obj["speed"].d;
        bool looping = anim.obj["looping"].b;
        for (auto &frame : anim.obj["array"].arr) {
            animFrames.emplace_back(frame.i);
        }
        auto &anim_name = anim.obj["name"].str;
        if (anim_name == "death") {
            death_anim_id = animSprite.addAnimation(anim_name, std::move(animFrames), speed, looping);
        }
        else {
            animSprite.addAnimation(anim_name, std::move(animFrames), speed, looping);
        }
    }

    animSprite.play("idle up-left");

    local_center = collider.collision_offset + collider.box.size / 2.f;
    
    // LOAD WEAPONs
    auto &json_weapon = config->doc["weapons"].obj;
    Texture weapon_tex = roommanager->loadTexture("weapons", json_weapon["spritesheet"].str.c_str());

    auto &json_bow = json_weapon["bow"].obj["texture coordinates"].arr;
    vec2i bow_texture_coordinates { json_bow[0].i, json_bow[1].i };
    // bow = Weapon(&roommanager->textures["weapons"], bow_texture_coordinates, in, collisionlayer, roommanager);

    auto &json_hitbox = json_weapon["arrow"].obj["hitbox"].arr;
    // bow.setHitBox({json_hitbox[0].i, json_hitbox[1].i, json_hitbox[2].i, json_hitbox[3].i});
    playerdata.basedamage = (float)json_weapon["bow"].obj["base_damage"].d;
    // bow.setDamage(playerdata.basedamage);

    // CHECKBOX
    checkboxsize = vec2(300, 300);
    checkbox = Collision(-checkboxsize.x / 2.f, -checkboxsize.y / 2.f, checkboxsize.x / 2.f, checkboxsize.y / 2.f, Collision::LAYER::PLAYER);
    checkbox.setDebugColor(YELLOW);

    // PLAYER GUI
    playerdata.expneeded = nextLevelExp(playerdata.level);
    Texture playerbar_tex = roommanager->loadTexture("playerbar", "Assets/lifeexpbar.png");

    // roommanager->getPlayerGui()->setBarTexture(&roommanager->textures["playerbar"]);
    // roommanager->getPlayerGui()->setTotalLife(playerdata.totallife);
    // roommanager->getPlayerGui()->setNextLevelExp(playerdata.expneeded);
}

void Player::move(vec2 offset) {
    GameObject::move(offset);
    // bow.move(offset);
    checkbox.moveCollision(offset);
}

void Player::setPosition(vec2 pos) {
    animSprite.sprite.pos = pos;
    collider.setPosition(pos + collider.collision_offset);
    // bow.setPosition(pos + local_center);
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
        animSprite.play("death");
        return;
    }
    flashing.isflashing = true;

    // roommanager->getPlayerGui()->updateLifeValue(playerdata.life);
}

void Player::giveExp(float e) {
    std::cout << "exp given: " << e << " current exp: " << playerdata.exp << " needed: " << playerdata.expneeded << "\n";
    playerdata.exp += e;
    if (playerdata.exp >= playerdata.expneeded) {
        roommanager->playSound("level-up");

        float remain = playerdata.exp - playerdata.expneeded;
        // roommanager->getPlayerGui()->updateExpValue(playerdata.expneeded);
        playerdata.exp = remain;
        // roommanager->getPlayerGui()->remainExpValue(remain);

        levelUp();
    }
    else {
        // roommanager->getPlayerGui()->updateExpValue(playerdata.exp);
    }
}

void Player::levelUp() {
    playerdata.level++;
    playerdata.expneeded = nextLevelExp(playerdata.level);
    // roommanager->getPlayerGui()->setNextLevelExp(playerdata.expneeded);

    playerdata.totallife += playerdata.baselife;
    playerdata.life = playerdata.totallife;
    // roommanager->getPlayerGui()->setTotalLife(playerdata.totallife);
    // roommanager->getPlayerGui()->updateLifeValue(playerdata.life);
    
    // bow.setDamage(bow.getDamage() + 2.5f * playerdata.level);
}

float Player::nextLevelExp(int level) {
    return level * 6.f;
}

void Player::reset() {
    playerdata.level = 1;
    playerdata.exp = 0.f;
    playerdata.totallife = playerdata.baselife;
    playerdata.life = playerdata.baselife;
    // bow.setDamage(playerdata.basedamage);
    playerdata.expneeded = nextLevelExp(playerdata.level);
    // roommanager->getPlayerGui()->updateExpValue(playerdata.exp);
    // roommanager->getPlayerGui()->setNextLevelExp(playerdata.expneeded);
    animSprite.play("idle up-left");
}

void Player::handleInput(float dt) {
    oldVel = vel;
    if (canmove) {
        vel = vec2(0, 0);

        vel.x = (float)(IsKeyDown(KeyRight) - IsKeyDown(KeyLeft));
        vel.y = (float)(IsKeyDown(KeyDown)  - IsKeyDown(KeyUp));

        printf("vel: %.3f %.3f\n", vel.x, vel.y);

        vec2 mousepos = roommanager->getMousePos();
        vec2 center = animSprite.sprite.pos + local_center;

        center.x = mousepos.x - center.x;
        center.y = mousepos.y - center.y;
        angle_to_mouse = UsefulFunc::atan2(center.y, center.x) * 180.f / (float)M_PI + 180.f;
        // bow.setAngle(angle_to_mouse);

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

        vel.normalise();
        vel *= speed;

        std::string anim = "idle ";
        bool ismoving = vel != vec2(0, 0);
        if (ismoving) anim = "walk ";

        anim += directionnames[last_direction];

        if (IsKeyPressed(KEY_ESCAPE)) {
            // roommanager->textures["pausescreenshot"].create(w->getSize().x, w->getSize().y);
            // roommanager->textures["pausescreenshot"].update(*w);

            roommanager->moveMenu("pause");
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && ismoving) {
            canmove = false;
            isdodging = true;
            anim = "dodge ";

            if (vel.y < 0) anim += "up-";
            else           anim += "down-";

            if (vel.x < 0) anim += "left";
            else           anim += "right";

            collider.collisionlayer = Collision::LAYER::NONE;
        }

        animSprite.play(anim);
        // bow.handleInput(dt);
    }
    else if (isdodging) {
        if (!animSprite.playing) {
            canmove = true;
            isdodging = false;
            collider.collisionlayer = Collision::LAYER::PLAYER;
        }
    }
}

bool Player::animationCallback(int anim_id) {
    if (anim_id == death_anim_id) {
        roommanager->moveMenu("deathscreen");
        return true;
    }

    return false;
}

void Player::update(float dt) {
    animSprite.update(dt);
    // bow.update(dt);
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

    animSprite.play(anim);
    animSprite.update(dt);
}

void Player::draw() {
    // if (!draw_weapon_over && !isdodging) bow.draw();
    animSprite.draw();
    // if (draw_weapon_over && !isdodging) bow.draw();
    // bow.drawArrows(w);
}

void Player::drawDebug(){
    GameObject::drawDebug();
    // bow.drawDebug();
    checkbox.drawDebug();
}

void Player::setVisible(bool v) {
    GameObject::setVisible(v);
    // bow.setColor(flashing.color[v]);
}