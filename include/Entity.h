#pragma once

#include "Animation.h"
#include "AnimatedSprite.h"

enum EntityType {
    donkey,
    player,
    block,
    ladder,
    coin,
    peach,
    unknown
};

class Entity {
    public:
        Entity(bool isAnimated, EntityType type) :
                isAnimated(isAnimated),
                m_type(type)
        {}
    public:
        sf::Sprite m_sprite;
        sf::Vector2u m_size;
        sf::Vector2f m_position;
        EntityType m_type = EntityType::unknown;
        Animation* currentAnimation;
        AnimatedSprite animatedSprite;
        bool isAnimated = false;
        bool isMoving = false;

        // Enemy only
        bool m_bLeftToRight = true;
        int m_times = 0;
};

