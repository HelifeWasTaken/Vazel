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
    EntityManagerException::EntityManagerException(std::string &e)
    {
        _e += e;
    }

    const char *EntityManagerException::what(void) const noexcept
    {
        return _e.c_str();
    }

    EntityManagerExceptionFindEntityError::EntityManagerExceptionFindEntityError(const char *e)
        : EntityManagerException::EntityManagerException(e) {}

    const Entity EntityManager::createEntity(void)
    {
        Entity e;

        _entity_map[e] = ComponentSignature();
        return e;
    }

    EntityManager &EntityManager::setSignature(const Entity &e, const ComponentSignature &signature)
    {
        try
        {
            _entity_map[e] = signature;
            return *this;
        }
        catch (...)
        {
            char buf[BUFSIZ] = {0};
            snprintf(buf, sizeof(buf) - 1, "EntityManager::setSignature: %lu does not exist", e.getId());
            throw EntityManagerExceptionFindEntityError(buf);
        }
    }

    const ComponentSignature &EntityManager::getSignature(const Entity &e) const
    {
        try
        {
            return _entity_map[e];
        }
        catch (...)
        {
            char buf[BUFSIZ] = {0};
            snprintf(buf, sizeof(buf) - 1, "EntityManager::getSignature: %lu does not exist", e.getId());
            throw EntityManagerExceptionFindEntityError(buf);
        }
    }

    const EntityMap &EntityManager::getMap(void) const
    {
        return _entity_map;
    }
}
