#pragma once

#include <unordered_map>
#include <iostream>
#include <string>
#include <memory>
#include <exception>

#include "Vazel/Components/Component.hpp"
#include "Vazel/Entity/Entity.hpp"

namespace vazel
{
    /**
     * @brief ComponentsManagerException class
     *      Exception class for ComponentsManager
     */
    class ComponentManagerException : std::exception
    {
        private:
            std::string _e = "ComponentManagerException: ";

        public:
            ComponentManagerException(const char *e);

        public:
            const char *what();
    };

    /**
     * @brief ComponentManagerRegisterError class determines if a component is already registered
     *        or the number of components is already at Vazel::Component::MAX
     */
    class ComponentManagerRegisterError : public ComponentManagerException
    {
        public:
            ComponentManagerRegisterError(const char *e);
    };

    /**
     * @brief ComponentMap is unordered_map of Components
     *
     */
    using ComponentMap = std::unordered_map<const char *, ComponentType>;
    using ComponentArray = std::array<Component, VAZEL_MAX_COMPONENTS>;

    class ComponentManager
    {
        private:
            ComponentMap _components_map;
            ComponentSignature _aviable_signatures;
            std::unordered_map<Entity, ComponentArray> _entity_to_components;

            ComponentType _getAviableComponentIndex();

        public:
            const ComponentMap &getComponentMap() const;
            const ComponentSignature &getComponentSignature() const;

            /**
             * @brief shows the current state of the ComponentManager
             *
             */
            void showState() const;

            /**
             * @brief registerComponent registers a Component in the ComponentManager with the typeid
             *        as key and the ComponentType as value
             *
             * @tparam T The componentType to register
             */
            template <typename T>
                void Register()
                {
                    const char *name = typeid(T).name();
                    const ComponentType aviableIndex = _getAviableComponentIndex();

                    if (_components_map.find(name) != _components_map.end()) {
                        std::string err = "You cannot Register the same component twice: ";
                        err += name;
                        throw ComponentManagerRegisterError(err.c_str());
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
                void unRegister()
                {
                    const char *name = typeid(T).name();

                    try {
                        _aviable_signatures.set(_components_map.at(name), false);
                    } catch (...) {
                        std::string err = "You cannot unregister a component that is not registered: ";
                        err += name;
                        throw ComponentManagerRegisterError(err.c_str());
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
                ComponentType getComponentType()
                {
                    try {
                        return _components_map.at(typeid(T).name());
                    } catch (...) {
                        std::string err = "You cannot get a component that is not registered: ";
                        err += typeid(T).name();
                        throw ComponentManagerRegisterError(err.c_str());
                    }
                }

            /**
             * @brief addComponent adds a Component to the ComponentManager
             *       and attach it to the Entity
             *
             */
            template <typename T>
                void attachComponent(Entity& e)
                {
                    try {
                        const ComponentType componentType = _components_map.at(typeid(T).name());
                        auto it = _entity_to_components.find(e);
                        if (it == _entity_to_components.end())
                            _entity_to_components.emplace(e, ComponentArray());
                        if (it->second[componentType].get<T>() != nullptr)
                            throw ComponentManagerRegisterError("You cannot attach a component that is already attached");
                        it->second[componentType].make<T>();
                    } catch (...) {
                        std::string err = "You cannot attach a component that is not registered: ";
                        err += typeid(T).name();
                        throw ComponentManagerRegisterError(err.c_str());
                    }
                }

            /**
             * @brief removeComponent removes a Component from the ComponentManager
             *      and detach it from the Entity
             */
            template <typename T>
                void detachComponent(Entity& e)
                {
                    const ComponentType type = getComponentType<T>();
                    auto it = _entity_to_components.find(e);

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
