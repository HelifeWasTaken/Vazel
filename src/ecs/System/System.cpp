/**
 * src/ecs/System/System.cpp
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
#include "Vazel/ecs/System/System.hpp"

namespace vazel
{
    namespace ecs
    {

        void unimplementedOnUpdateSystem(ComponentManager &cm, const Entity &e)
        {
            (void)e;
            (void)cm;
        }

        void System::__addEntity(const Entity &entity,
                                 const ComponentSignature &signature)
        {
            if (isValidSignature(signature, _signature)) {
                _entities.emplace(entity);
            }
        }

        System::System(const std::string &tag)
            : _tag(tag)
            , _updater(unimplementedOnUpdateSystem)
        {
        }

        System::System(const char *tag)
            : _tag(tag)
            , _updater(unimplementedOnUpdateSystem)
        {
        }

        void System::updateValidEntities(EntityManager &emanager)
        {
            for (const auto &t : emanager.getMap()) {
                auto it = _entities.find(t.first);
                if (it == _entities.end()) {
                    __addEntity(t.first, t.second);
                } else if (isValidSignature(t.second, _signature) == false) {
                    _entities.erase(it);
                }
            }
        }

        void System::setOnUpdate(systemUpdate updater)
        {
            _updater = updater;
        }

        const ComponentSignature &System::getSignature(void) const
        {
            return _signature;
        }

        const std::string &System::getTag(void) const
        {
            return _tag;
        }

        void System::update(ComponentManager &cm)
        {
            for (auto it : _entities) {
                _updater(cm, it);
            }
        }

        void System::addDependency(const ComponentType &type)
        {
            _signature.set(type, true);
        }

        void System::addDependency(EntityManager &emanager,
                                   const ComponentType &type)
        {
            addDependency(type);
            updateValidEntities(emanager);
        }

    } // namespace ecs
} // namespace vazel
