#include "Vazel/Components/ComponentsManager.hpp"

namespace vazel
{
    ComponentType ComponentManager::_getAviableComponentIndex()
    {
        for (ComponentType i = 0; i != Component::MAX; i++)
            if (_aviable_signatures.test(i) == false)
                return i;
        throw ComponentManagerRegisterError("You registered already the maximum of Component");
    }

    const ComponentMap &ComponentManager::getComponentMap() const
    {
        return _components_map;
    }

    const ComponentSignature &ComponentManager::getComponentSignature() const
    {
        return _aviable_signatures;
    }

    std::ostream &operator<<(std::ostream &os, const ComponentManager &cManager)
    {
        const auto &map = cManager.getComponentMap();

        os << "----- Components Begin -----" << std::endl;
        for (auto &it : map)
            os << "Name: [" << it.first << "] Id: [" << it.second << "]" << std::endl;
        os << "------ Components End ------" << std::endl;
        return os;
    }

    void ComponentManager::showState() const
    {
        std::cout << *this;
    }

    ComponentManagerException::ComponentManagerException(const char *e)
    {
        _e += e;
    }

    const char *ComponentManagerException::what()
    {
        return _e.c_str();
    }

    ComponentManagerFindComponentError::ComponentManagerFindComponentError(const char *e)
        : ComponentManagerException(e) {}

    ComponentManagerRegisterError::ComponentManagerRegisterError(const char *e)
        : ComponentManagerException(e) {}
};
