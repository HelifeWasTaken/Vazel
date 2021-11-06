#pragma once

#include <bitset>
#include <stdlib.h>

/**
 * @brief The maximum number of components.
 *
 */
#define VAZEL_MAX_COMPONENTS 30

namespace vazel
{
    /**
     * @brief Base class for all components.
     *
     */
    using ComponentType = uint16_t;

    class Component
    {
        private:
            void *_data = nullptr;

        public:

            /**
             * @brief Construct a new T object
             *
             * @tparam T The type of the component.
             */
            template<typename T>
                void make()
                {
                    _data = calloc(sizeof(T), 1);
                    if (_data == NULL)
                        throw std::bad_alloc();
                }

            /**
             * @brief Destroy the _data object but not the Component class.
             */
            void remove(void);

            /**
             * @brief Construct a new Component object
             *
             */
            Component(void) = default;

            ~Component(void);

            /**
             * @brief Gets the data object casted to the given type.
             *
             * @tparam T The type to cast the data object to.
             * @return T* The data object casted to the given type as a pointer.
             */
            template<typename T>
                T *get(void)
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
