/**
 * src/Components/Component.cpp
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
#include "Vazel/Components/Component.hpp"

namespace vazel
{
    void Component::remove(void)
    {
        _data.reset();
    }

    Component::~Component(void)
    {
        remove();
    }

    ComponentExistsException::ComponentExistsException(const std::string& e)
        : _e(e) {}

    const char* ComponentExistsException::what(void) const noexcept
    {
        return _e.c_str();
    }
}
