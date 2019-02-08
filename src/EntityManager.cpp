#include "pch.h"
#include "EntityManager.h"


std::list<std::shared_ptr<Entity>> EntityManager::m_Entities;

std::shared_ptr<Entity> EntityManager::GetPlayer() {
    for (std::shared_ptr<Entity> entity : EntityManager::m_Entities) {
        if (!entity->m_enabled) {
            continue;
        }

        if (entity->m_type == EntityType::player) {
            return entity;
        }
    }

    return nullptr;
}

std::list<std::shared_ptr<Entity>> EntityManager::GetCoins() {
    std::list<std::shared_ptr<Entity>> coins;

    for (auto const& entity : EntityManager::m_Entities) {
        if (!entity->m_enabled) {
            continue;
        }

        if (entity->m_type == EntityType::coin) {
            coins.push_back(entity);
        }
    }

    return coins;
}

void EntityManager::RemoveCoin(std::shared_ptr<Entity> coin) {
    EntityManager::m_Entities.remove(coin);
}