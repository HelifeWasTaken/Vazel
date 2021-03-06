/**
 * include/Vazel/UUID.hpp
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

#include <random>

namespace vazel
{

    // A UUID should be a 128-bit number that is used to identify a resource.
    // This is a 64-bit number that is generated by the system and should be
    // unique for each resource. 128-bits implentation should come soon.
    //
    // We can still use __int128_t but it would not become standard
    using UUID = uint_fast64_t;

    /**
     * @brief Generates a random UUID.
     *
     * @return UUID A random UUID.
     */
    UUID makeUUID(void);

} // namespace vazel
