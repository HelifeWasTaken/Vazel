/**
 * include/Vazel/World/World.hpp
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

#include "Vazel/Components/ComponentsManager.hpp"
#include "Vazel/Entity/EntityManager.hpp"
#include "Vazel/System/System.hpp"

#include <list>

namespace vazel
{

    /**
     * @brief WorldException is the base class for all exceptions thrown by the
     * World class.
     *
     */
    class WorldException : public VException
    {
      private:
        std::string _e = "WorldException: ";

      public:
        /**
         * @brief Construct a new World Exception object
         *
         * @param e the exception message
         */
        WorldException(const std::string &e);

        /**
         * @brief Get the what object.
         *
         * @return const char* The error message.
         */
        const char *what() const throw() override;
    };

    // TODO: Change std::list<std::pair<System, std::string>> to
    // std::unordered_set<System> And get system tag info with System::getTag()
    /**
     * @brief The World class
     *
     */
    class World
    {
      private:
        std::list<std::pair<System, std::string>> _systems;
        ComponentManager _componentManager;
        EntityManager _entityManager;

      public:
        /**
         * @brief Construct a new World object
         *
         */
        World(void) = default;

        /**
         * @brief Destroy the World object
         *
         */
        ~World(void) = default;

        /**
         * @brief Create a Entity object
         *
         * @return Entity
         */
        Entity createEntity(void);

        /**
         * @brief Remove an Entity from the world
         *
         * @param e The entity to remove.
         * @return World & A reference to *this.
         */
        World &removeEntity(Entity &e);

        /**
         * @brief Remove a System from the world
         *
         * @param tag The tag of the system to remove.
         * @return World & A reference to *this.
         */
        World &removeSystem(std::string &tag);

        /**
         * @brief Add a System to the world
         *
         * @param sys The system to add. (with at least one depenedency)
         * @param tag The tag of the system to add.
         * @return World& A reference to *this.
         */
        World &addSystem(System &sys, std::string &tag);

        /**
         * @brief Add a dependency to a System
         *
         * @tparam T The type of the dependency.
         * @param tagname The tagname of the system to add the dependency to.
         * @return World& A reference to *this.
         */
        template <typename T>
        World &addSystemDependency(std::string tagname)
        {
            // TODO: Change Exception raised
            for (auto &it : _systems) {
                if (it.second == tagname) {
                    it.first.addDependency<T>(_entityManager,
                                              _componentManager);
                    return *this;
                }
            }
            std::string err = "World::addSystemDependency: Could not find a "
                              "system with tag: \"";
            err += tagname + "\"";
            throw WorldException(err);
        }

        /**
         * @brief Remove a dependency from a System
         *
         * @tparam T The type of the dependency.
         * @param tagname The tagname of the system to remove the dependency
         * from.
         * @return World& A reference to *this.
         */
        template <typename T>
        World &removeSystemDependency(std::string tagname)
        {
            // TODO: Change Exception raised
            for (auto &it : _systems) {
                if (it.second == tagname) {
                    it.first.removeDependency<T>(_entityManager,
                                                 _componentManager);
                    if (it.first.getSignature() == 0)
                        _systems.remove(it);
                    return *this;
                }
            }
            std::string err = "World::removeSystemDependency: Could not find a "
                              "system with tag: \"";
            err += tagname + "\"";
            throw WorldException(err);
        }

        /**
         * @brief Get the Entity Signature object
         *
         * @param e The entity to get the signature from.
         * @return const ComponentSignature& The signature.
         */
        const ComponentSignature &getEntitySignature(Entity &e);

        /**
         * @brief Get the Component Manager Signature object
         *
         * @return const ComponentSignature& The signature.
         */
        const ComponentSignature &getComponentManagerSignature(void) const;

        /**
         * @brief Update the systems entities containers with the current
         * entities in the EntityManager
         *
         * @return World& A reference to *this.
         */
        World &updateSystemsEntities(void);

        /**
         * @brief Update the systems with System::update for each system
         *
         * @return World& A reference to *this.
         */
        World &updateSystem(void);

        /**
         * @brief Register a Component to the ComponentManager
         *
         * @tparam T The type of the component.
         * @return World& A reference to *this.
         */
        template <typename T>
        World &registerComponent(void)
        {
            _componentManager.registerComponent<T>();
            return *this;
        }

        /**
         * @brief Attach a Component to an Entity
         *
         * @tparam T The type of the component.
         * @param e The entity to attach the component to.
         * @param data The data of the component.
         * @return World& A reference to *this.
         */
        template <typename T>
        World &attachComponentToEntity(Entity &e, T &data)
        {
            _componentManager.attachComponent<T>(e, data);
            _entityManager.getSignature(e).set(
                _componentManager.getComponentType<T>(e), true);
            return updateSystemsEntities();
        }

        /**
         * @brief Attach a Component to an Entity (with a default value for T)
         *
         * @tparam T The type of the component.
         * @param e The entity to attach the component to.
         * @return World& A reference to *this.
         */
        template <typename T>
        World &attachComponentToEntity(Entity &e)
        {
            T data = T();
            return attachComponentToEntity<T>(e, data);
        }

        /**
         * @brief Detach a Component from an Entity
         *
         * @tparam T The type of the component.
         * @param e The entity to detach the component from.
         * @return World& A reference to *this.
         */
        template <typename T>
        World &removeComponentFromEntity(Entity &e)
        {
            _componentManager.detachComponent<T>(e);
            _entityManager.getSignature(e).set(
                _componentManager.getComponentType<T>(e), false);
            return updateSystemsEntities();
        }

        /**
         * @brief Clear completely the World instance
         *
         */
        void clearWorld(void);
    };

} // namespace vazel
