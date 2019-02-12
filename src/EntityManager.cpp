#include "pch.h"
#include "EntityManager.h"


std::list<std::shared_ptr<Entity>> EntityManager::m_Entities;

std::list<std::shared_ptr<Entity>> EntityManager::GetCoins() {
    std::list<std::shared_ptr<Entity>> coins;

    for (auto const& entity : EntityManager::m_Entities) {
        if (entity->m_type == EntityType::coin) {
            coins.push_back(entity);
        }
    }

    return coins;
}

void EntityManager::RemoveCoin(std::shared_ptr<Entity> coin) {
    EntityManager::m_Entities.remove(coin);
}

std::list<std::shared_ptr<Entity>> EntityManager::GetFloors() {
    std::list<std::shared_ptr<Entity>> floors;

    for (auto const& entity : EntityManager::m_Entities) {
        if (entity->m_type == EntityType::block) {
            floors.push_back(entity);
        }
    }

    return floors;
}

std::list<std::shared_ptr<Entity>> EntityManager::GetLadders() {
    std::list<std::shared_ptr<Entity>> floors;

    for (auto const& entity : EntityManager::m_Entities) {
        if (entity->m_type == EntityType::scale) {
            floors.push_back(entity);
        }
    }

    return floors;
}