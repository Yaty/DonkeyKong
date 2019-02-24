#pragma once

#include "Entity.h"
#include <list>

class Donkey: public Entity {
    public:
        Donkey() : Entity(true, EntityType::donkey) {}
        sf::Texture spriteSheet;
        Animation chest;
};