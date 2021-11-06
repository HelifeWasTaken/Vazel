/**
 * include/Vazel/Entity/EntityManager.hpp
 * Copyright (c) 2021 Mattis DALLEAU <mattisdalleau@gmail.com>
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
#pragma once

#include <exception>
#include <iostream>
#include <string.h>
#include <unordered_map>

#include "Vazel/Entity/Entity.hpp"
#include "Vazel/Components/Component.hpp"

namespace vazel
{
    class EntityManagerException : public std::exception
    {

        private:
            std::string _e = "EntityManagerError: ";

        public:

            EntityManagerException(std::string& e)
            {
                _e += e;
            }

            const char *what(void) const noexcept override
            {
                return _e.c_str();
            }
    };

    class EntityManagerExceptionFindEntityError : public EntityManagerException
    {
        public:
            EntityManagerExceptionFindEntityError(const char *e)
                : EntityManagerException(e) {}
    };

    using EntityMap = std::unordered_map<Entity, ComponentSignature>;

    class EntityManager
    {
        private:
            EntityMap _entity_map;

        public:
            Entity createEntity(void)
            {
                Entity e;

                _entity_map[e] = ComponentSignature();
                return e;
            }

            void setSignature(Entity& e, const ComponentSignature& signature)
            {
                _entity_map[e] = signature;
            }

            ComponentSignature& getSignature(Entity& e)
            {
                return _entity_map[e];
            }

            const EntityMap& getMap(void) const
            {
                return _entity_map;
            }
    };
}
