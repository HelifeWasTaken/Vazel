/**
 * include/Vazel/Components/ComponentsManager.hpp
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

#include <unordered_map>
#include <iostream>
#include <string>
#include <memory>
#include <exception>
#include <cstdio>

#include "Vazel/Components/Component.hpp"
#include "Vazel/Entity/Entity.hpp"

namespace vazel
{
    /**
     * @brief ComponentsManagerException class
     *      Exception class for ComponentsManager
     */
    class ComponentManagerException : public std::exception
    {
        private:
            std::string _e = "ComponentManagerException: ";

        public:
            /**
             * @brief Construct a new Component Manager Exception object
             *
             * @param e Exception message
             */
            ComponentManagerException(const std::string& e);

        public:
            /**
             * @brief Get the Exception Message object
             *
             * @return const char*
             */
            const char *what() const noexcept override;
    };

    /**
     * @brief ComponentManagerRegisterError class determines if a component is already registered
     *        or the number of components is already at Vazel::Component::MAX
     */
    class ComponentManagerRegisterError : public ComponentManagerException
    {
        public:
            /**
             * @brief Construct a new Component Manager Register Error object
             *
             * @param e Exception message
             */
            ComponentManagerRegisterError(const std::string& e);
    };

    /**
     * @brief ComponentMap is an unordered_map of Components based on their typename
     *
     */
    using ComponentMap = std::unordered_map<const char *, ComponentType>;

    /**
     * @brief ComponentArray is an array of Components of size VAZEL_MAX_COMPONENTS
     */
    using ComponentArray = std::array<Component, VAZEL_MAX_COMPONENTS>;

    /**
     * @brief ComponentsManager class
     *      Manages all components of an Entity
     */
    class ComponentManager
    {
        private:
            ComponentMap _components_map;
            ComponentSignature _aviable_signatures;
            std::unordered_map<Entity, ComponentArray> _entity_to_components;

            /**
             * @brief get the component type from the component name
             *
             * @return ComponentType the component type
             */
            ComponentType _getAviableComponentIndex(void);

        public:

            /**
             * @brief Construct a new Component Manager object
             */
            ComponentManager(void) = default;

            /**
             * @brief Destroy the Component Manager object
             */
            ~ComponentManager(void) = default;

            const ComponentMap &getComponentMap(void) const;
            const ComponentSignature &getComponentSignature(void) const;

            /**
             * @brief shows the current state of the ComponentManager
             *
             */
            void showState(void) const;

            /**
             * @brief registerComponent registers a Component in the ComponentManager with the typeid
             *        as key and the ComponentType as value
             *
             * @tparam T The componentType to register
             */
            template <typename T>
                void registerComponent(void)
                {
                    const char *name = typeid(T).name();
                    const ComponentType aviableIndex = _getAviableComponentIndex();

                    if (_components_map.find(name) != _components_map.end()) {
                        std::string err = "You cannot Register the same component twice: ";
                        err += name;
                        throw ComponentManagerRegisterError(err);
                    }
                    _aviable_signatures.set(aviableIndex, true);
                    _components_map.emplace(name, aviableIndex);
                }

            /**
             * @brief unregister removes a Component from the ComponentManager
             *
             * @tparam T The componentType to remove
             */
            template <typename T>
                void unregisterComponent(void)
                {
                    const char *name = typeid(T).name();

                    try {
                        _aviable_signatures.set(_components_map.at(name), false);
                    } catch (...) {
                        std::string err = "You cannot unregister a component that is not registered: ";
                        err += name;
                        throw ComponentManagerRegisterError(err);
                    }
                    _components_map.erase(name);
                }

            /**
             * @brief Get the Component Type object
             *
             * @tparam T The componentType to get
             * @return ComponentType an index in the ComponentSignature (bitset) where the Component is stored
             */
            template <typename T>
                const ComponentType& getComponentType(void) const
                {
                    const char *name = typeid(T).name();

                    try {
                        return _components_map.at(name);
                    } catch (...) {
                        std::string err = "You cannot get a component that is not registered: ";
                        err += name;
                        throw ComponentManagerRegisterError(err);
                    }
                }

            /**
             * @brief Should always be called to register the entity
             *
             */
            void onEntityCreate(const Entity& e)
            {
                if (_entity_to_components.find(e) != _entity_to_components.end()) {
                    char buf[200] = {0};
                    std::snprintf(buf, sizeof(buf) - 1,
                            "Entity(%lu) is already registered"
                            " in the _entity_to_components map", e.getId());
                    throw ComponentManagerException(std::string(buf));
                }
                _entity_to_components.emplace(e, ComponentArray());
            }

            /**
             * @brief Should always be called to unregister an entity
             *
             */
            void onEntityDestroy(const Entity& e)
            {
                const auto& it = _entity_to_components.find(e);

                if (it == _entity_to_components.end()) {
                    char buf[200] = {0};
                    std::snprintf(buf, sizeof(buf) - 1,
                            "Entity(%lu) is not registered in "
                            "the _entity_to_components map", e.getId());
                    throw ComponentManagerException(std::string(buf));
                }
                _entity_to_components.erase(it->first);
            }

            /**
             * @brief addComponent adds a Component to the ComponentManager
             *       and attach it to the Entity
             *
             */
            template <typename T>
                void attachComponent(const Entity& e)
                {
                    const char *name = typeid(T).name();

                    try {
                        const ComponentType componentType = _components_map.at(name);
                        auto it = _entity_to_components.find(e);

                        if (it == _entity_to_components.end()) {
                            std::string err = "You cannot attach a component to a non registered Entity(";
                            err += e.getId();
                            err += ")";
                            throw ComponentManagerException(err);
                        }
                        if (it->second[componentType].hasValue())
                            throw ComponentManagerException("You cannot attach a component that is already attached");
                        it->second[componentType].make<T>();
                    } catch (...) {
                        std::string err = "You cannot attach a component that is not registered: ";
                        err += name;
                        throw ComponentManagerRegisterError(err);
                    }
                }

            /**
             * @brief removeComponent removes a Component from the ComponentManager
             *      and detach it from the Entity
             */
            template <typename T>
                void detachComponent(const Entity& e)
                {
                    const ComponentType type = getComponentType<T>();
                    const auto it = _entity_to_components.find(e);

                    if (it == _entity_to_components.end()) {
                        std::string err = "You cannot detach a component to an non attached Entity";
                        err += typeid(T).name();
                        throw ComponentManagerRegisterError(err.c_str());
                    }
                    it->second[type].remove();
                    // maybe throw an exception if the component is not attached
                    // Depends if it should work like a free
                    // .remove on Component should check already if component is null
                }

            /**
             * @brief getComponent gets the Component of a specifically attached Entity
             *
             */
            template <typename T>
                T& getComponent(const Entity& e)
                {
                    const char *name = typeid(T).name();

                    try {
                        const ComponentType componentType = _components_map.at(name);
                        const auto it = _entity_to_components.find(e);

                        if (it == _entity_to_components.end())
                            throw std::runtime_error("");
                        return it->second[componentType].get<T>();
                    } catch (std::exception& err) {
                        char buf[200] = {0};
                        std::snprintf(buf, sizeof(buf) - 1,
                                "Entity(%lu) is not registered"
                                " with a component or Component(%s) was not found",
                                e.getId(),
                                name
                        );
                        throw ComponentManagerException(std::string(buf));
                    }
                }
    };

    /**
     * @brief Logging function for ComponentManager
     *
     * @param os The output stream
     * @param cManager The ComponentManager to log
     * @return std::ostream& The output stream
     */
    std::ostream &operator<<(std::ostream &os, const ComponentManager &cManager);

}
