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

            EntityManagerException(char const *e)
            {
                _e += e;
            }

            const char *what()
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
