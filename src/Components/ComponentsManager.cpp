/**
 * src/Components/ComponentsManager.cpp
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
#include "Vazel/Components/ComponentsManager.hpp"

namespace vazel
{

    ComponentType ComponentManager::_getAviableComponentIndex(void)
    {
        for (ComponentType i = 0; i != VAZEL_MAX_COMPONENTS; i++) {
            if (_aviable_signatures.test(i) == false) {
                return i;
            }
        }
        throw ComponentManagerRegisterError(
            "ComponentManager::_getAviableComponentIndex: You registered "
            "already the maximum of Component");
    }

    const ComponentMap &ComponentManager::getComponentMap(void) const
    {
        return _components_map;
    }

    const ComponentSignature &ComponentManager::getComponentSignature(
        void) const
    {
        return _aviable_signatures;
    }

    std::ostream &operator<<(std::ostream &os, const ComponentManager &cManager)
    {
        const auto &map = cManager.getComponentMap();

        os << "----- Components Begin -----" << std::endl;
        for (auto &it : map) {
            os << "Name: [" << it.first << "] Id: [" << it.second << "]"
               << std::endl;
        }
        os << "------ Components End ------" << std::endl;
        return os;
    }

    void ComponentManager::showState(void) const
    {
        std::cout << *this;
    }

    ComponentManagerException::ComponentManagerException(const std::string &e)
    {
        _e += e;
    }

    const char *ComponentManagerException::what(void) const throw()
    {
        return _e.c_str();
    }

    ComponentManagerRegisterError::ComponentManagerRegisterError(
        const std::string &e)
        : ComponentManagerException(e)
    {
    }

    ComponentManager &ComponentManager::onEntityCreate(const Entity &e)
    {
        if (_entity_to_components.find(e) != _entity_to_components.end()) {
            char buf[BUFSIZ] = { 0 };
            std::snprintf(buf, sizeof(buf) - 1,
                          "ComponenentManager::onEntityCreate: "
                          "Entity(%lu) is already registered"
                          " in the _entity_to_components map",
                          e.getId());
            throw ComponentManagerException(std::string(buf));
        }
        _entity_to_components.emplace(e, ComponentArray());
        return *this;
    }

    ComponentManager &ComponentManager::onEntityDestroy(const Entity &e)
    {
        const auto &it = _entity_to_components.find(e);

        if (it == _entity_to_components.end()) {
            char buf[BUFSIZ] = { 0 };
            std::snprintf(buf, sizeof(buf) - 1,
                          "ComponenentManager::onEntityCreate: "
                          "Entity(%lu) is not registered in "
                          "the _entity_to_components map",
                          e.getId());
            throw ComponentManagerException(std::string(buf));
        }
        _entity_to_components.erase(it->first);
        return *this;
    }

    void ComponentManager::clear(void)
    {
        _components_map.clear();
        _entity_to_components.clear();
        _aviable_signatures &= 0;
    }

}; // namespace vazel
