#include "Vazel/Components/ComponentsManager.hpp"

namespace vazel
{
    ComponentType ComponentManager::_getAviableComponentIndex(void)
    {
        for (ComponentType i = 0; i != VAZEL_MAX_COMPONENTS; i++)
            if (_aviable_signatures.test(i) == false)
                return i;
        throw ComponentManagerRegisterError("You registered already the maximum of Component");
    }

    const ComponentMap &ComponentManager::getComponentMap(void) const
    {
        return _components_map;
    }

    const ComponentSignature &ComponentManager::getComponentSignature(void) const
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

    void ComponentManager::showState(void) const
    {
        std::cout << *this;
    }

    ComponentManagerException::ComponentManagerException(const std::string& e)
    {
        _e += e;
    }

    const char *ComponentManagerException::what(void) const noexcept
    {
        return _e.c_str();
    }

    ComponentManagerRegisterError::ComponentManagerRegisterError(const std::string& e)
        : ComponentManagerException(e) {}
};
