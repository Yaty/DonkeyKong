#pragma once

#include "Entity.h"
#include <list>

class Barrel: public Entity {
public:
    Barrel() : Entity(true, EntityType::barrel) {}
    Animation rolling;
    Animation falling;
    sf::Time fallingTime;
    int direction = 1;
};