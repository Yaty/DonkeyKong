#pragma once

#include "Entity.h"

class Mario: public Entity {
    public:
        Mario() : Entity(true, EntityType::player) {}

        bool isMovingUp = false;
        bool isMovingDown = false;
        bool isMovingRight = false;
        bool isMovingLeft = false;
        bool isFacingRight = true;
        bool isFacingLeft = false;
        bool isJumping = false;
        sf::Time lastJump;
        Animation walkingAnimationLeft;
        Animation walkingAnimationRight;
        sf::Sprite standingRightRect;
        sf::Sprite standingLeftRect;
        bool isOnBottomOfLadder() const;


};

