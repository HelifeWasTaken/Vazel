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

#include "Vazel/ecs/Components/ComponentsManager.hpp"
#include "Vazel/ecs/Entity/EntityManager.hpp"
#include "Vazel/ecs/System/System.hpp"

#include <list>

namespace vazel
{
    namespace ecs
    {

        /**
         * @brief WorldException is the base class for all exceptions thrown by
         * the World class.
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

        /**
         * @brief The World class
         *
         */
        class World
        {
          private:
            std::vector<std::unique_ptr<System>> _systems;
            ComponentManager _componentManager;
            EntityManager _entityManager;

            std::vector<std::unique_ptr<System>>::iterator
                __getSystemIteratorFromTag(const char *tag)
            {
                return std::find_if(_systems.begin(), _systems.end(),
                                    [&](std::unique_ptr<System> &s) {
                                        return s->getTag() == tag;
                                    });
            }

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
             */
            void removeEntity(Entity &e);

            /**
             * @brief Remove a System from the world
             *
             * @param tag The tag of the system to remove.
             */
            void removeSystem(std::string &tag);

            /**
             * @brief Remove a System from the world
             *
             * @param tag The tag of the system to remove.
             */
            void removeSystem(const char *tag);

            /**
             * @brief Add a System to the world
             *
             * @param sys The system to add. (with at least one depenedency)
             */
            void registerSystem(System &sys);

            /**
             * @brief Add a dependency to a System
             *
             * @tparam T The type of the dependency.
             * @param tag The tagname of the system to add the dependency to.
             * @return World& A reference to *this.
             */
            template <typename T>
            void addSystemDependency(const char *tag)
            {
                const auto it = __getSystemIteratorFromTag(tag);

                try {
                    (*it)->addDependency<T>(_entityManager, _componentManager);
                } catch (ComponentManagerException &e) {
                    throw WorldException(e.what());
                }
                std::string err =
                    "World::addSystemDependency: Could not find a "
                    "system with tag: \"";
                err += tag;
                err += "\"";
                throw WorldException(err);
            }

            /**
             * @brief Add a dependency to a System
             *
             * @tparam T The type of the dependency.
             * @param tag The tagname of the system to add the dependency to.
             * @return World& A reference to *this.
             */
            template <typename T>
            void addSystemDependency(std::string &tag)
            {
                addSystemDependency<T>(tag.c_str());
            }

            /**
             * @brief Remove a dependency from a System
             *
             * @tparam T The type of the dependency.
             * @param tag The tagname of the system to remove the dependency
             * from.
             */
            template <typename T>
            void removeSystemDependency(const char *tag)
            {
                const auto it = __getSystemIteratorFromTag(tag);

                if (it != _systems.end()) {
                    try {
                        (*it)->removeDependency<T>(_entityManager,
                                                   _componentManager);
                    } catch (ComponentManagerException &e) {
                        throw WorldException(e.what());
                    }
                    if ((*it)->getSignature() == 0)
                        _systems.erase(it);
                    return;
                }
                std::string err =
                    "World::removeSystemDependency: Could not find a "
                    "system with tag: \"";
                err += tag;
                err += "\"";
                throw WorldException(err);
            }

            /**
             * @brief Remove a dependency from a System
             *
             * @tparam T The type of the dependency.
             * @param tag The tagname of the system to remove the dependency
             * from.
             */
            template <typename T>
            void removeSystemDependency(std::string &tag)
            {
                removeSystemDependency<T>(tag.c_str());
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
             * @brief Get the Component Type object
             *
             * @tparam T Type of the component that you want to get
             * @return const ComponentType& The ComponentType
             */
            template <typename T>
            const ComponentType &getComponentType(void) const
            {
                return _componentManager.getComponentType<T>();
            }

            /**
             * @brief Update the systems entities containers with the current
             * entities in the EntityManager
             */
            void updateSystemsEntities(void);

            /**
             * @brief Update the systems with System::update for each system
             */
            void updateSystem(void);

            /**
             * @brief Register a Component to the ComponentManager
             *
             * @tparam T The type of the component.
             */
            template <typename T>
            const ComponentType registerComponent(void)
            {
                return _componentManager.registerComponent<T>();
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
            void attachComponent(Entity &e, T &data)
            {
                _componentManager.attachComponent<T>(e, data);
                _entityManager.getSignature(e).set(
                    _componentManager.getComponentType<T>(), true);
                updateSystemsEntities();
            }

            /**
             * @brief Attach a Component to an Entity (with a default value for
             * T)
             *
             * @tparam T The type of the component.
             * @param e The entity to attach the component to.
             * @return World& A reference to *this.
             */
            template <typename T>
            void attachComponent(Entity &e)
            {
                T data = T();
                attachComponent<T>(e, data);
            }

            /**
             * @brief Detach a Component from an Entity
             *
             * @tparam T The type of the component.
             * @param e The entity to detach the component from.
             * @return World& A reference to *this.
             */
            template <typename T>
            void detachComponent(Entity &e)
            {
                _componentManager.detachComponent<T>(e);
                _entityManager.getSignature(e).set(
                    _componentManager.getComponentType<T>(), false);
                updateSystemsEntities();
            }

            template <typename T>
            T &getComponent(Entity &e)
            {
                return _componentManager.getComponent<T>(e);
            }

            /**
             * @brief Clear completely the World instance
             *
             */
            void clearWorld(void);
        };

    } // namespace ecs
} // namespace vazel
