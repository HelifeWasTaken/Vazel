/**
 * src/World/World.cpp
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
#include "Vazel/World/World.hpp"

namespace vazel
{

    WorldException::WorldException(const std::string &e)
    {
        _e += e;
    }

    const char *WorldException::what(void) const throw()
    {
        return _e.c_str();
    }

    Entity World::createEntity(void)
    {
        Entity e = _entityManager.createEntity();
        _componentManager.onEntityCreate(e);
        return e;
    }

    void World::removeEntity(Entity &e)
    {
        _entityManager.setSignature(e, ComponentSignature());
        updateSystemsEntities();
        _entityManager.destroyEntity(e);
        _componentManager.onEntityDestroy(e);
    }

    void World::removeSystem(const char *tag)
    {
        for (const auto &it : _systems) {
            if (it->getTag() == tag) {
                _systems.remove(it);
                return;
            }
        }
        std::string err =
            "World::removeSystem: Cannot find system with tag: \"";
        err += tag;
        err += "\"";
        throw WorldException(err);
    }

    void World::removeSystem(std::string &tag)
    {
        removeSystem(tag.c_str());
    }

    void World::registerSystem(System &sys)
    {
        if (sys.getSignature() == 0) {
            throw WorldException("World::addSystem: System signature is 0");
        }
        for (const auto &it : _systems) {
            if (it->getTag() == sys.getTag()) {
                std::string err = "World::addSystem: A system with tag: \"";
                err += sys.getTag() + "\" already exists";
                throw WorldException(err);
            }
        }
        sys.updateValidEntities(_entityManager);
        _systems.push_front(std::make_unique<System>(sys));
    }

    const ComponentSignature &World::getEntitySignature(Entity &e)
    {
        return _entityManager.getSignature(e);
    }

    const ComponentSignature &World::getComponentManagerSignature(void) const
    {
        return _componentManager.getComponentSignature();
    }

    void World::updateSystemsEntities(void)
    {
        for (auto &it : _systems) {
            it->updateValidEntities(_entityManager);
        }
    }

    void World::updateSystem(void)
    {
        for (auto &it : _systems) {
            it->update(_componentManager);
        }
    }

    void World::clearWorld(void)
    {
        _systems.clear();
        _componentManager.clear();
        _entityManager.clear();
    }

} // namespace vazel
