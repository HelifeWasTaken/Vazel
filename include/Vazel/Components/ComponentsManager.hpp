#include <unordered_map>
#include <iostream>
#include <string>
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
     * @brief ComponentManagerFindComponentError class that
     *        determines that a Component has been requested but not found
     */

    class ComponentManagerFindComponentError : public ComponentManagerException
    {
        public:
            ComponentManagerFindComponentError(const char *e);
    };

    /**
     * @brief ComponentMap is unordered_map of Components
     *
     */
    using ComponentMap = std::unordered_map<const char *, ComponentType>;

    class ComponentManager
    {
        private:
            ComponentMap _components_map;
            ComponentSignature _aviable_signatures;

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
                void registerComponent()
                {
                    const char *name = typeid(T).name();
                    const ComponentType aviableIndex = _getAviableComponentIndex();

                    if (_components_map.find(name) != _components_map.end()) {
                        std::string e = "You cannot Register the same component twice: ";
                        e += name;
                        throw ComponentManagerRegisterError(e.c_str());
                    }
                    _aviable_signatures.set(aviableIndex, true);
                    _components_map.emplace(name, aviableIndex);
                }

            /**
             * @brief Get the Component object
             *
             * @tparam T The componentType to get
             * @return const ComponentMap::iterator The current position of the Component in the ComponentMap
             */
            template <typename T>
                const ComponentMap::iterator getComponent()
                {
                    const char *name = typeid(T).name();
                    const auto r = _components_map.find(name);

                    if (r == _components_map.end()) {
                        std::string s("You did not registered component of type -> ");
                        s += name;
                        throw ComponentManagerFindComponentError(s.c_str());
                    }
                    return r;
                }

            /**
             * @brief removeComponent removes a Component from the ComponentManager
             *
             * @tparam T The componentType to remove
             */
            template <typename T>
                void removeComponent()
                {
                    const auto it = getComponent<T>();

                    _aviable_signatures.set(it->second, false);
                    _components_map.erase(it);
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
                    return getComponent<T>()->second;
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
