#include <EntityManager.h>
#include "Mario.h"
#include "Game.h"
#include <math.h>

sf::Rect<float> Mario::getBounds() const {
    return {
        m_position.x,
        m_position.y,
        (float) m_size.x ,
        (float) m_size.y
    };
}

bool Mario::isOnTheFloor() const {
    const auto floors = EntityManager::GetFloors();
    const auto playerBounds = getBounds();

    const auto playerFeetY = playerBounds.top + playerBounds.height;

    for (auto const& floor : floors) {
        const auto floorGlobalBounds = floor.get()->m_sprite.getGlobalBounds();
        const auto floorY = floorGlobalBounds.top;

        if (fabs(floorY - playerFeetY) < 5) {
            return true;
        }
    }

    return false;
}

std::list<std::shared_ptr<Entity>> Mario::getEntitiesInTheSameXAxis(std::list<std::shared_ptr<Entity>> entities) const {
    const auto floors = EntityManager::GetFloors();
    const auto playerBounds = getBounds();
    const auto playerX = playerBounds.left;
    const auto playerXEnd = playerBounds.left + playerBounds.width;

    std::list<std::shared_ptr<Entity>> entitiesInTheSameXAxis;

    for (auto const& entity : entities) {
        const auto entityGlobalBounds = entity.get()->m_sprite.getGlobalBounds();
        const auto entityX = entityGlobalBounds.left;
        const auto entityXEnd = entityGlobalBounds.left + entityGlobalBounds.width;

        if (playerX <= entityXEnd && entityX <= playerXEnd) {
            entitiesInTheSameXAxis.push_back(entity);
        }
    }

    return entitiesInTheSameXAxis;
}

float Mario::getDistanceFromFloor() const {
    const auto floors = EntityManager::GetFloors();
    const auto playerBounds = getBounds();
    const auto floorsInTheSameXAxis = getEntitiesInTheSameXAxis(EntityManager::GetFloors());

    float distance = SCREEN_HEIGHT;
    const auto playerFeetY = playerBounds.top + playerBounds.height;

    for (auto const& floor : floorsInTheSameXAxis) {
        const auto floorGlobalBounds = floor.get()->m_sprite.getGlobalBounds();
        auto diff = floorGlobalBounds.top - playerFeetY;

        if (diff >= 0 && diff < distance) {
            distance = diff;
        }
    }

    return distance;
}

bool Mario::isOnTopOfLadder() const {
    const auto ladders = EntityManager::GetLadders();
    const auto playerBounds = getBounds();
    const auto playerFeetY = playerBounds.top + playerBounds.height;
    const auto laddersInTheSameXAxis = getEntitiesInTheSameXAxis(EntityManager::GetLadders());

    for (auto const& ladder : laddersInTheSameXAxis) {
        const auto ladderGlobalBounds = ladder.get()->m_sprite.getGlobalBounds();
        const auto diff = ladderGlobalBounds.top - playerFeetY;

        if (fabs(diff) < 5 && isOnTheFloor()) {
            return true;
        }
    }

    return false;
}
