#include <EntityManager.h>
#include "Mario.h"
#include "Game.h"
#include <math.h>

bool Mario::isOnBottomOfLadder() const {
    if (!onTheFloor) {
        return false;
    }

    const auto ladders = EntityManager::GetLadders();
    const auto playerBounds = getBounds();
    const auto playerFeetY = playerBounds.top + playerBounds.height;
    const auto laddersInTheSameXAxis = getEntitiesInTheSameXAxis(EntityManager::GetLadders());

    for (auto const& ladder : laddersInTheSameXAxis) {
        if (fabs(ladder.get()->m_position.y + ladder.get()->m_size.y - playerFeetY) <= 10) {
            return true;
        }
    }

    return false;
}