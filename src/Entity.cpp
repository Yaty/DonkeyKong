#include "pch.h"
#include "Entity.h"
#include <EntityManager.h>
#include <math.h>

bool Entity::isOnTopOfLadder() const {
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

std::list<std::shared_ptr<Entity>> Entity::getEntitiesInTheSameXAxis(std::list<std::shared_ptr<Entity>> entities) const {
    const auto floors = EntityManager::GetFloors();
    const auto thisEntityBounds = getBounds();
    const auto thisEntityX = thisEntityBounds.left;
    const auto thisEntityXEnd = thisEntityBounds.left + thisEntityBounds.width;

    std::list<std::shared_ptr<Entity>> entitiesInTheSameXAxis;

    for (auto const& entity : entities) {
        const auto entityGlobalBounds = entity.get()->m_sprite.getGlobalBounds();
        const auto entityX = entityGlobalBounds.left;
        const auto entityXEnd = entityGlobalBounds.left + entityGlobalBounds.width;

        if (thisEntityX <= entityXEnd && entityX <= thisEntityXEnd) {
            entitiesInTheSameXAxis.push_back(entity);
        }
    }

    return entitiesInTheSameXAxis;
}


bool Entity::isOnTheFloor() const {
    const auto playerBounds = getBounds();
    const auto playerFeetY = playerBounds.top + playerBounds.height;
    const auto floorsInTheSameXAxis = getEntitiesInTheSameXAxis(EntityManager::GetFloors());

    for (auto const& floor : floorsInTheSameXAxis) {
        const auto floorGlobalBounds = floor.get()->m_sprite.getGlobalBounds();
        const auto floorY = floorGlobalBounds.top;

        if (fabs(floorY - playerFeetY) <= 10) {
            return true;
        }
    }

    return false;
}

sf::Rect<float> Entity::getBounds() const {
    return {
            m_position.x,
            m_position.y,
            (float) m_size.x ,
            (float) m_size.y
    };
}