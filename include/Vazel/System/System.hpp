#include "Vazel/Components/ComponentsManager.hpp"
#include <functional>

namespace vazel {

    static bool isValidSignature(const ComponentSignature& signature)
    {
        return (signature & _signature) == _signature;
    }

    void unimplementedOnUpdateSystem(ComponentManager& cm, const Entity& e)
    {
        (void)e;
        (void)cm;
    }

    using systemUpdate = std::function<void(ComponentManager&, const Entity&)>;

    class System {

        private:
            ComponentSignature _signature;
            std::string _tag;
            systemUpdate _on_update;
            std::list<Entity> _entities;

            void __updateCurrentEntities(const EntityManager& emanager)
            {
                std::remove_if(_entities.begin(), _entities.end(), [&emanager](Entity& e){
                    return isValidSignature(emanager.getSignature(e));
                });
            }

            void __addEntity(const Entity& entity, const ComponentSignature& signature)
            {
                if (isValidSignature(signature)) {
                    _entities.push_front(entity);
                }
            }

        public:
            System::System(std::string tag, systemUpdate updater=unimplementedOnUpdateSystem)
                : _updater(updater), _tag(tag);
            
            ~System::System() = default;

            System& addEntities(const EntityManager& emanager)
            {
                for (const auto& [entity, signature] : emanager) {
                    if (_entities.find(entity) != _entities.end()) {
                        __addEntity(entity, signature);
                    }
                }
                return *this;
            }

            System& setOnUpdate(const systemUpdate updater)
            {
                _updater = updater;
                return *this;
            }

            const ComponentSignature& getSignature(void) const
            {
                return _signature;
            }

            const std::string& getTag(void) const
            {
                return _tag;
            }

            template<typename T>
                System& addDependency(const ComponentManager& cmanager)
                {
                    _signature.set(cmanager.getComponentType<T>());
                    return *this;
                }
            
            template<typename T>
                System& removeDependency(const EntityManager& emanager, const ComponentManager& cmanager)
                {
                    _signature.set(cmanager.getComponentType<T>(), 0);
                    __updateCurrentEntities(emanager);
                    return *this;
                }

            void update(ComponentManager& cm) const
            {
                for (const Entity& entity : _entities) {
                    _updater(cm, entity);
                }
            }
    };

}