/**
 * src/UUID.cpp
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
#include "Vazel/UUID.hpp"

namespace vazel
{
    static std::random_device s_randomDevice;
    static std::mt19937_64 s_rEngine(s_randomDevice());
    static std::uniform_int_distribution<UUID> s_uniformDistribution;

    UUID makeUUID(void)
    {
        return s_uniformDistribution(s_rEngine);
    }
} // namespace vazel
