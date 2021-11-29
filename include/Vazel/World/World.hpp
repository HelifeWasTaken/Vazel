#include "Vazel/Components/ComponentsManager.hpp"
#include "Vazel/Entity/EntityManager.hpp"
#include "Vazel/System/System.hpp"

#include <list>

namespace vazel
{

    class World
    {
        private:
            std::list<std::pair<System, std::string>> _systems;
            ComponentManager _componentManager;
            EntityManager _entityManager;

        public:
            Entity createEntity(void)
            {
                Entity e = _entityManager.createEntity();
                _componentManager.onEntityCreate(e);
                return e;
            }

            void removeEntity(Entity &e)
            {
                _entityManager.setSignature(e, ComponentSignature());
                updateSystemsEntities();
                _entityManager.destroyEntity(e);
                _componentManager.onEntityDestroy(e);
            }

            void addSystem(System &sys, std::string &tag)
            {
                // TODO: Change Exception raised
                if (sys.getSignature() == 0) {
                    throw std::runtime_error("Signature must have at least one dependency component");
                }
                for (const auto& it : _systems) {
                    if (it.second == tag) {
                         throw std::runtime_error(std::string("A system with \"") + std::string(tag) + "\" as tagname is already registered");
                    }
                }
                sys.updateValidEntities(_entityManager);
                _systems.push_front(std::make_pair(sys, tag));
            }

            template<typename T>
            void addSystemDependency(std::string tagname)
            {
                // TODO: Change Exception raised
                for (auto& it : _systems) {
                    if (it.second == tagname) {
                        it.first.addDependency<T>(_entityManager, _componentManager);
                        return;
                    }
                }
                throw std::runtime_error(std::string("Could not find a system with \"") + std::string(tagname) + "\" as tag");
            }

            template<typename T>
            void removeSystemDependency(std::string tagname)
            {
                // TODO: Change Exception raised
                for (auto& it : _systems) {
                    if (it.second == tagname) {
                        it.first.removeDependency<T>(_entityManager, _componentManager);
                        if (it.first.getSignature() == 0)
                            _systems.erase(it);
                        return;
                    }
                }
                throw std::runtime_error(std::string("Could not find a system with \"") + std::string(tagname) + "\" as tag");
            }

            const ComponentSignature &getEntitySignature(Entity &e)
            {
                return _entityManager.getSignature(e);
            }

            const ComponentSignature &getComponentManagerSignature(Entity &e) const
            {
                return _componentManager.getComponentSignature();
            }

            void updateSystemsEntities(void)
            {
                for (auto& it : _systems) {
                    it.first.updateValidEntities(_entityManager);
                }
            }

            template<typename T>
                void registerComponent(void)
                {
                    _componentManager.registerComponent<T>();
                }

            template<typename T>
                void attachComponentToEntity(Entity &e)
                {
                    _componentManager.attachComponent<T>(e);
                    _entityManager.getSignature(e).set(_componentManager.getComponentType<T>(e), true);
                    updateSystemsEntities();
                }

            template<typename T>
                void removeComponentFromEntity(Entity &e)
                {
                    _entityManager.getSignature(e).set(_componentManager.getComponentType<T>(e), false);
                    updateSystemsEntities();
                }

    };

}
