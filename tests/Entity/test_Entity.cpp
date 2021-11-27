/**
 * tests/Entity/test_Entity.cpp
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
#include <gtest/gtest.h>
#include <Vazel/Entity/Entity.hpp>
#include <unordered_set>

TEST(EntitiesGeneration, GenerateOneEntity)
{
    std::cout << "EntityRandomId: " << vazel::Entity().getId() << std::endl;
}

TEST(EntitiesGeneration, MakeSureThereIsNoCollisions)
{
    std::unordered_set<vazel::UUID> _set;

    for (size_t i = 0; i != 1000; i++)
    {
        vazel::Entity e;
        vazel::UUID res = std::hash<vazel::Entity>()(e);
        GTEST_ASSERT_EQ(_set.find(res), _set.end());
        _set.emplace(res);
    }
}
