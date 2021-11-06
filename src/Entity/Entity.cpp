#include "Vazel/UUID.hpp"
#include "Vazel/Entity/Entity.hpp"

namespace vazel
{
    Entity::Entity(void)
        : _uuid(makeUUID()) {}

    Entity::Entity(const Entity &uuid)
        : _uuid(uuid) {}

    UUID Entity::getId(void) const
    {
        return _uuid;
    }

}
