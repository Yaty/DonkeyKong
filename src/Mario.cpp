#include <EntityManager.h>
#include "Mario.h"
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