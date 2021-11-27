/**
 * include/Vazel/Components/Component.hpp
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
#pragma once

#include <bitset>
#include <stdlib.h>
#include <any>

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
            std::any _data;

        public:

            /**
             * @brief Construct a new T object
             *
             * @tparam T The type of the component.
             */
            template<typename T>
                void make()
                {
                    if (_data.has_value())
                        throw ComponentExistsException("Component is not null and you are trying to make a new one.");
                    _data = T();
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
                    if (_data.has_value())
                        throw ComponentExistsException("Component is not null and you are trying to make a new one.");
                    _data = data;
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
             * @brief Wrapper around get_ref
             * @tparam Type to use around get_ref
             * @return T& A reference by the object return by get_reg
             */
            template<typename T>
                T& get(void)
                {
                    return std::any_cast<T&>(_data);
                }

            bool hasValue(void) const { return _data.has_value(); }
    };

    /**
     * @brief Component that can be attached to an entity.
     * We use a bitmask to know which components are attached to an entity.
     *
     */
    using ComponentSignature = std::bitset<VAZEL_MAX_COMPONENTS>;

}
