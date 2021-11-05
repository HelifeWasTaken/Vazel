#include "Vazel/Entity/Entity.hpp"

namespace vazel
{
    static std::random_device s_randomDevice;
    static std::mt19937_64 s_rEngine(s_randomDevice());
    static std::uniform_int_distribution<uint64_t> s_uniformDistribution;

    Entity::Entity(void) : _uuid(s_uniformDistribution(s_rEngine)) {}
    Entity::Entity(const Entity &uuid) : _uuid(uuid) {}
}