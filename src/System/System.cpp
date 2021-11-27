/**
 * src/System/System.cpp
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
#include "Vazel/System/System.hpp"

namespace vazel
{
    void unimplementedOnUpdateSystem(ComponentManager &cm, const Entity &e)
    {
        (void)e;
        (void)cm;
    }

    void System::__updateCurrentEntities(EntityManager &emanager)
    {
        auto it = _entities.begin();

        while (it != _entities.end())
        {
           if (isValidSignature(emanager.getSignature(it->first), _signature))
           {
                it = _entities.erase(it);
           }
           else
           {
                ++it;

           }
        }
    }

    void System::__addEntity(const Entity &entity, const ComponentSignature &signature)
    {
        if (isValidSignature(signature, _signature))
        {
            _entities[entity] = signature;
        }
    }

    System::System(std::string tag, systemUpdate updater = unimplementedOnUpdateSystem)
        : _tag(std::move(tag)), _updater(updater) {}

    System &System::addEntities(const EntityManager &emanager)
    {
        for (const auto &t : emanager.getMap())
        {
            if (_entities.find(t.first) != _entities.end())
            {
                __addEntity(t.first, t.second);
            }
        }
        return *this;
    }

    System &System::setOnUpdate(const systemUpdate updater)
    {
        _updater = updater;
        return *this;
    }

    const ComponentSignature &System::getSignature(void) const
    {
        return _signature;
    }

    const std::string &System::getTag(void) const
    {
        return _tag;
    }

    void System::update(ComponentManager &cm) const
    {
        for (const auto &t : _entities)
        {
            _updater(cm, t.first);
        }
    }
};
