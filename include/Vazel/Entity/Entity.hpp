#include <random>

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
            uint64_t _uuid;

        public:
            Entity();
            Entity(const Entity &uuid);

            /**
             * @brief Wrapper to get the UUID
             *
             * @return uint64_t The UUID
             */
            operator uint64_t() const { return _uuid; }

            /**
             * @brief Wrapper to get the UUID
             *
             * @return uint64_t The UUID
             */
            uint64_t getId() const { return _uuid; }
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
     * @return uint64_t The result of the hash
     */
    uint64_t operator()(const vazel::Entity &uuid) const
    {
        return std::hash<uint64_t>()((uint64_t)uuid);
    }
};
