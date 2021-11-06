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

    class ComponentExistsException : public std::exception
    {
        private:
            std::string _e;

        public:
            ComponentExistsException(const std::string& e);
            const char* what() const noexcept override;
    };

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
                    if (_data != nullptr)
                        throw ComponentExistsException("Component is not null and you are trying to make a new one.");
                    _data = calloc(sizeof(T), 1);
                    if (_data == nullptr)
                        throw std::bad_alloc();
                }

            /**
             * @brief Construct a new T object with an initial value.
             *
             * @tparam T The type of the component.
             * @param data The initial value of the component.
             */
            template<typename T>
                void make(T data)
                {
                    make<T>();
                    memcpy(_data, &data, sizeof(T));
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

            /**
             * @brief Destroy the Component object
             */
            ~Component(void);

            /**
             * @brief Gets the data object casted to the given type.
             *
             * @tparam T The type to cast the data object to.
             * @return T* The data object casted to the given type as a pointer.
             */
            template<typename T>
                T *get_ptr(void)
                {
                    return static_cast<T *>(_data);
                }

            /**
             * @brief Gets the data object casted to the given type.
             *
             * @tparam T The type to cast the data object to.
             * @return T& A reference to the data object casted to the given type.
             */
            template<typename T>
                T& get_ref(void)
                {
                    return *reinterpret_cast<T*>(_data);
                }

            /**
             * @brief Wrapper around get_ref
             * @tparam Type to use around get_ref
             * @return T& A reference by the object return by get_reg
             */
            template<typename T>
                T& get(void)
                {
                    return *reinterpret_cast<T*>(_data);
                }
    };

    /**
     * @brief Component that can be attached to an entity.
     * We use a bitmask to know which components are attached to an entity.
     *
     */
    using ComponentSignature = std::bitset<VAZEL_MAX_COMPONENTS>;

}
