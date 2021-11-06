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
