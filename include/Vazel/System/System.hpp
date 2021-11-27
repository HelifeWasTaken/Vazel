/**
 * include/Vazel/System/System.hpp
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

#include "Vazel/Components/ComponentsManager.hpp"
#include "Vazel/Entity/EntityManager.hpp"
#include <functional>
#include <list>
#include <algorithm>

namespace vazel
{

    using systemUpdate = std::function<void(ComponentManager &, const Entity &)>;

    /**
     * @brief System class is a collection of entities that can be updated at the same time
     */
    class System
    {

    private:
        ComponentSignature _signature;
        std::string _tag;
        systemUpdate _updater;
        EntityMap _entities;

        /**
         * @brief Update all entities of the system
         *
         * @param emanager EntityManager
         */
        void __updateCurrentEntities(const EntityManager &emanager);

        /**
         * @brief Add an entity to the system
         *
         * @param entity Entity
         * @param signature ComponentSignature
         */
        void __addEntity(const Entity &entity, const ComponentSignature &signature);

    public:

        /**
         * @brief Construct a new System object
         *
         * @param tag System tag
         * @param updater System update function
         */
        System(std::string tag, systemUpdate updater);

        /**
         * @brief Destroy the System object
         *        Remove all entities from the system
         */
        ~System() = default;

        /**
         * @brief Add all entities from the EntityManager that match the system signature
         *
         * @param emanager EntityManager
         * @return System& Reference to the class itself
         */
        System &addEntities(const EntityManager &emanager);

        /**
         * @brief Set the system update function
         *
         * @param updater System update function
         * @return System& Reference to the class itself
         */
        System &setOnUpdate(const systemUpdate updater);

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
         * @brief Add a dependency to the system (A component required so the entity can be attached)
         *        if one of the already attached entities has not the new required dependency required,
         *        the entity is removed from the system
         *
         * @tparam T Component type
         * @param cmanager ComponentManager
         * @return System& Reference to the class itself
         */
        template <typename T>
        System &addDependency(const ComponentManager &cmanager)
        {
            _signature.set(cmanager.getComponentType<T>());
            __updateCurrentEntities(emanager);
            return *this;
        }

        /**
         * @brief Remove a dependency from the system (A component required so the entity can be attached)
         *        if one of the Entities from the EntityManager matches the new Signature, the entity is added to the system
         *
         * @tparam T Component type
         * @param emanager EntityManager to get the entities from
         * @param cmanager ComponentManager to get the component type from
         * @return System& Reference to the class itself
         */
        template <typename T>
        System &removeDependency(const EntityManager &emanager, const ComponentManager &cmanager)
        {
            _signature.set(cmanager.getComponentType<T>(), 0);
            addEntities(emanager);
            return *this;
        }

        /**
         * @brief Update all entities of the system
         *
         * @param cm ComponentManager to get the entities com from
         */
        void update(ComponentManager &cm) const;
    };

}
