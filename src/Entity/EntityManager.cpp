/**
 * src/Entity/EntityManager.cpp
 * Copyright (c) 2021 Mattis Dalleau <mattis.dalleau@epitech.eu>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Vazel/Entity/EntityManager.hpp"

namespace vazel
{

    EntityManagerException::EntityManagerException(const std::string &e)
    {
        _e += e;
    }

    const char *EntityManagerException::what(void) const throw()
    {
        return _e.c_str();
    }

    EntityManagerExceptionFindEntityError::
        EntityManagerExceptionFindEntityError(const std::string &e)
        : EntityManagerException::EntityManagerException(e)
    {
    }

    Entity EntityManager::createEntity(void)
    {
        Entity e = Entity();

        _entity_map[e] = ComponentSignature();
        return e;
    }

    void EntityManager::destroyEntity(Entity &e)
    {
        if (_entity_map.find(e) == _entity_map.end()) {
            char buf[BUFSIZ] = { 0 };
            snprintf(buf, sizeof(buf) - 1,
                     "EntityManager::destroyEntity: Entity %lu does not exist",
                     e.getId());
            throw EntityManagerExceptionFindEntityError(buf);
        }
        _entity_map.erase(e);
        return;
    }

    void EntityManager::setSignature(const Entity &e,
                                     const ComponentSignature &signature)
    {
        try {
            _entity_map[e] = signature;
            return;
        } catch (...) {
            char buf[BUFSIZ] = { 0 };
            snprintf(buf, sizeof(buf) - 1,
                     "EntityManager::setSignature: %lu does not exist",
                     e.getId());
            throw EntityManagerExceptionFindEntityError(buf);
        }
    }

    ComponentSignature &EntityManager::getSignature(const Entity &e)
    {
        try {
            return _entity_map[e];
        } catch (...) {
            char buf[BUFSIZ] = { 0 };
            snprintf(buf, sizeof(buf) - 1,
                     "EntityManager::getSignature: %lu does not exist",
                     e.getId());
            throw EntityManagerExceptionFindEntityError(buf);
        }
    }

    const EntityMap &EntityManager::getMap(void) const
    {
        return _entity_map;
    }

    void EntityManager::clear(void)
    {
        _entity_map.clear();
    }

} // namespace vazel
