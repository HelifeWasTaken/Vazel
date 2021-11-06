#pragma once

#include <bitset>
#include <stdlib.h>

namespace vazel
{

    /**
     * @brief Base class for all components.
     *
     */
    using ComponentType = uint16_t;

    /**
     * @brief The maximum number of components.
     *
     */
    #define VAZEL_MAX_COMPONENTS 300

    class Component
    {
        private:
            void *_data = nullptr;

        public:

            /**
             * @brief Construct a new Component object.
             *
             */
            template<typename T>
            void make()
            {
                _data = calloc(sizeof(T), 1);
                if (_data == NULL)
                    throw std::bad_alloc();
            }

            void remove()
            {
                if (_data != nullptr)
                    free(_data);
                _data = nullptr;
            }

            Component() = default;

            ~Component()
            {
                remove();
            }

            template<typename T>
            T *get()
            {
                return static_cast<T *>(_data);
            }
    };

    /**
     * @brief Component that can be attached to an entity.
     * We use a bitmask to know which components are attached to an entity.
     *
     */
    using ComponentSignature = std::bitset<VAZEL_MAX_COMPONENTS>;

}
