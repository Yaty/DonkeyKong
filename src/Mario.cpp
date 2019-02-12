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
    bool isFalling = true;
    bool isJumping = false;
    bool isOnLadder = false;
    sf::Time lastJump;
    Animation walkingAnimationLeft;
    Animation walkingAnimationRight;
    sf::Sprite standingRightRect;
    sf::Sprite standingLeftRect;
};