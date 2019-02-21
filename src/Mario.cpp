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

bool Mario::isOnTheFloor() const {
    const auto playerBounds = getBounds();
    const auto playerFeetY = playerBounds.top + playerBounds.height;
    const auto floorsInTheSameXAxis = getEntitiesInTheSameXAxis(EntityManager::GetFloors());

    for (auto const& floor : floorsInTheSameXAxis) {
        const auto floorGlobalBounds = floor.get()->m_sprite.getGlobalBounds();
        const auto floorY = floorGlobalBounds.top;

        if (fabs(floorY - playerFeetY) <= 5) {
            return true;
        }
    }

    return false;
}

bool Mario::isOnTopOfLadder() const {
    if (!onTheFloor) {
        return false;
    }

    const auto ladders = EntityManager::GetLadders();
    const auto playerBounds = getBounds();
    const auto playerFeetY = playerBounds.top + playerBounds.height;
    const auto laddersInTheSameXAxis = getEntitiesInTheSameXAxis(EntityManager::GetLadders());

    for (auto const& ladder : laddersInTheSameXAxis) {
        const auto ladderGlobalBounds = ladder.get()->m_sprite.getGlobalBounds();

        if (fabs(ladderGlobalBounds.top - playerFeetY) <= 8) {
            return true;
        }
    }

    return false;
}

bool Mario::isOnBottomOfLadder() const {
    if (!onTheFloor) {
        return false;
    }

    const auto ladders = EntityManager::GetLadders();
    const auto playerBounds = getBounds();
    const auto playerFeetY = playerBounds.top + playerBounds.height;
    const auto laddersInTheSameXAxis = getEntitiesInTheSameXAxis(EntityManager::GetLadders());

    for (auto const& ladder : laddersInTheSameXAxis) {
        if (fabs(ladder.get()->m_position.y + ladder.get()->m_size.y - playerFeetY) <= 5) {
            return true;
        }
    }

    return false;
}
