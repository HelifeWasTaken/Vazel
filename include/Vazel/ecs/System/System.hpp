/**
 * include/Vazel/ecs/System/System.hpp
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
#pragma once

#include "Vazel/ecs/Components/ComponentsManager.hpp"
#include "Vazel/ecs/Entity/EntityManager.hpp"

#include <algorithm>
#include <functional>
#include <list>
#include <unordered_set>

/**
 * @brief VAZEL_SYSTEM_UPDATE_LAMBDA is a macro to define a lambda function to
 * update a system The lambda functions can take paramters for [] [&] [=]
 * etc... It provides you directly the components manager and the associated
 * Entity
 */
#define VAZEL_SYSTEM_UPDATE_LAMBDA(componentManagerName, entityName, ...) \
    [__VA_ARGS__](vazel::ecs::ComponentManager & componentManagerName,    \
                  const vazel::ecs::Entity &entityName)

namespace vazel
{
    namespace ecs
    {

        using systemUpdate =
            std::function<void(ComponentManager &, const Entity &)>;

        /**
         * @brief System class is a collection of entities that can be updated
         * at the same time
         */
        class System
        {

          private:
            ComponentSignature _signature;
            std::string _tag;
            systemUpdate _on_update;
            std::unordered_set<Entity> _entities;

            /**
             * @brief Add an entity to the system
             *
             * @param entity Entity
             * @param signature ComponentSignature
             */
            void __addEntity(const Entity &entity,
                             const ComponentSignature &signature);

          public:
            /**
             * @brief Construct a new System object
             *
             * @param tag System tag
             */
            System(const std::string &tag);

            /**
             * @brief Construct a new System object
             *
             * @param tag System tag
             */
            System(const char *tag);

            /**
             * @brief Destroy the System object
             *        Remove all entities from the system
             */
            ~System(void) = default;

            /**
             * @brief Add all entities from the EntityManager that match the
             * system signature
             *
             * @param emanager EntityManager
             */
            void updateValidEntities(EntityManager &emanager);

            /**
             * @brief Set the system update function
             *
             * @param updater System update function
             */
            void setOnUpdate(systemUpdate updater);

            /**
             * @brief Get the system signature
             *
             * @return ComponentSignature System signature
             */
            const ComponentSignature &getSignature(void) const;

            /**
             * @brief Get the system tag
             *
             * @return std::string System tag
             */
            const std::string &getTag(void) const;

            /**
             * @brief Add a dependency to the system (Does not update the
             * Entities)
             *
             * @tparam T Component type
             * @param cmanager ComponentManager
             */
            template <typename T>
            void addDependency(const ComponentManager &cmanager)
            {
                _signature.set(cmanager.getComponentType<T>(), true);
            }

            /**
             * @brief Add a dependency to the system (A component required so
             * the entity can be attached) if one of the already attached
             * entities has not the new required dependency required, the
             * entity is removed from the system
             *
             * @tparam T Component type
             * @param cmanager ComponentManager
             */
            template <typename T>
            void addDependency(EntityManager &emanager,
                               const ComponentManager &cmanager)
            {
                addDependency<T>(cmanager);
                updateValidEntities(emanager);
            }

            /**
             * @brief Add a dependency from a ComponentType (does not update
             * Entities)
             * @param component Component Type to add as a Dependency
             */
            void addDependency(const ComponentType &type);

            /**
             * @brief Add a dependency from a ComponentType
             * @param component Component Type to add as a Dependency
             */
            void addDependency(EntityManager &emanager,
                               const ComponentType &type);

            /**
             * @brief Remove a dependency from the system (A component required
             * so the entity can be attached) if one of the Entities from the
             * EntityManager matches the new Signature, the entity is added to
             * the system
             *
             * @tparam T Component type
             * @param emanager EntityManager to get the entities from
             * @param cmanager ComponentManager to get the component type from
             */
            template <typename T>
            void removeDependency(EntityManager &emanager,
                                  const ComponentManager &cmanager)
            {
                _signature.set(cmanager.getComponentType<T>(), 0);
                updateValidEntities(emanager);
            }

            /**
             * @brief Remove a dependency from the system (Does not update the
             * entities)
             *
             * @tparam T Component type
             * @param cmanager ComponentManager to get the component type from
             */
            template <typename T>
            void removeDependency(const ComponentManager &cmanager)
            {
                _signature.set(cmanager.getComponentType<T>(), 0);
            }

            /**
             * @brief Update all entities of the system
             *
             * @param cm ComponentManager to get the entities com from
             */
            void onUpdate(ComponentManager &cm);
        };

    } // namespace ecs
} // namespace vazel
