#pragma once

#include "Animation.h"
#include "AnimatedSprite.h"
#include <list>

enum EntityType {
    donkey,
    player,
    block,
    ladder,
    coin,
    peach,
    barrel,
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
        bool isOnLadder = false;
        bool onBottomOfLadder = false;
        bool onTopOfLadder = false;
        bool onTheFloor = true;
        bool isFalling = true;
        bool isClimbing;
        sf::Rect<float> getBounds() const;
        std::list<std::shared_ptr<Entity>> getEntitiesInTheSameXAxis(std::list<std::shared_ptr<Entity>> entities) const;
        bool isOnTheFloor() const;
        bool isOnTopOfLadder() const;
        // Enemy only
        bool m_bLeftToRight = true;
        int m_times = 0;
};

