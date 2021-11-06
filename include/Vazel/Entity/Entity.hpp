/**
 * include/Vazel/Entity/Entity.hpp
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

#include <functional>

#include "Vazel/UUID.hpp"

namespace vazel
{
    /**
     * @brief The Entity class
     * This class is the base class for all entities in the game.
     * An entity is basically an ID that determines where it will stored in the EntityManager.
     */
    class Entity
    {
        private:
            UUID _uuid;

        public:
            /**
             * @brief Construct a new Entity object
             */
            Entity(void);

            /**
             * @brief Destroy the Entity object
             */
            Entity(const Entity &uuid);

            /**
             * @brief Wrapper to get the UUID
             *
             * @return UUID The UUID
             */
            UUID getId(void) const;


            /**
             * @brief Wrapper to get the UUID
             *
             * @return UUID The UUID
             */
            operator UUID() const
            {
                return getId();
            }

    };

}

/**
 * @brief The Hash of the Entity UUID
 *
 * @tparam Vazel::Entity
 */
template <>
struct std::hash<vazel::Entity>
{
    /**
     * @brief Hashes the UUID of the entity
     *
     * @param uuid The entity
     * @return UUID The result of the hash
     */
    vazel::UUID operator()(const vazel::Entity &uuid) const
    {
        return std::hash<vazel::UUID>()((vazel::UUID)uuid);
    }
};
