/**
 * include/Vazel/ecs/Entity/EntityManager.hpp
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

#include "Vazel/VException.hpp"
#include "Vazel/ecs/Components/Component.hpp"
#include "Vazel/ecs/Entity/Entity.hpp"

#include <iostream>
#include <string.h>
#include <unordered_map>

namespace vazel
{
    namespace ecs
    {

        /**
         * @brief EntityManagerException is the base class for all exceptions
         * thrown by EntityManager.
         *
         */
        class EntityManagerException : public VException
        {
          private:
            std::string _e = "EntityManagerError: ";

          public:
            /**
             * @brief Construct a new Entity Manager Exception object
             *
             * @param e The error message.
             */
            EntityManagerException(const std::string &e);

            /**
             * @brief Get the what object.
             *
             * @return const char* The error message.
             */
            const char *what(void) const noexcept override;
        };

        /**
         * @brief EntityManagerExceptionFindEntityError is thrown when an
         * entity is not found.
         *
         */
        class EntityManagerExceptionFindEntityError :
            public EntityManagerException
        {
          public:
            /**
             * @brief Construct a new Entity Manager Exception Find Entity
             * Error object
             *
             * @param e The error message.
             */
            EntityManagerExceptionFindEntityError(const std::string &e);
        };

        /**
         * @brief EntityMap is a map of entities and their components.
         *
         */
        using EntityMap = std::unordered_map<Entity, ComponentSignature>;

        class EntityManager
        {
          private:
            EntityMap _entity_map;

          public:
            /**
             * @brief Construct a new Entity Manager object
             *
             */
            EntityManager(void) = default;

            /**
             * @brief Destroy the Entity Manager object
             *
             */
            ~EntityManager(void) = default;

            /**
             * @brief Create a Entity object
             *
             * @return Entity
             */
            Entity createEntity(void);

            /**
             * @brief Destroy an entity.
             *
             * @param e The entity to destroy.
             * @throws EntityManagerExceptionFindEntityError if the entity is
             * not found.
             * @throws EntityManagerException if the entity is already
             * destroyed.
             */

            void destroyEntity(Entity &e);

            /**
             * @brief Set the Signature object
             *
             * @param e The entity.
             * @param signature The signature.
             */
            void setSignature(const Entity &e,
                              const ComponentSignature &signature);

            /**
             * @brief Get the Signature object
             *
             * @param e The entity.
             * @return ComponentSignature& The signature.
             */
            ComponentSignature &getSignature(const Entity &e);

            /**
             * @brief Get the Map object
             *
             * @return const EntityMap& The map (unordered).
             */
            const EntityMap &getMap(void) const;

            /**
             * @brief Clears the EntityMap of the EntityManager.
             *
             */
            void clear(void);
        };

    } // namespace ecs
} // namespace vazel
