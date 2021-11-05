#include <bitset>

namespace vazel
{

    /**
     * @brief Base class for all components.
     *
     */
    using ComponentType = uint16_t;

    class Component
    {
        public:
            /**
             * @brief The maximum number of components.
             * 
             */
            const static ComponentType MAX = 300;
    };

    /**
     * @brief Component that can be attached to an entity.
     * We use a bitmask to know which components are attached to an entity.
     * 
     */
    using ComponentSignature = std::bitset<Component::MAX>;

}
