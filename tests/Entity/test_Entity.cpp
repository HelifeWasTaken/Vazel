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
#include "Vazel/Entity/Entity.hpp"
#include "Vazel/Entity/EntityManager.hpp"

#include <gtest/gtest.h>
#include <unordered_set>

TEST(EntitiesGeneration, GenerateOneEntity)
{
    std::cout << "EntityRandomId: " << vazel::Entity().getId() << std::endl;
}

TEST(EntitiesGeneration, MakeSureThereIsNoCollisions)
{
    vazel::EntityMap entities;
    vazel::Entity e;

    for (size_t i = 0; i != 100000; i++) {
        e = vazel::Entity();
        GTEST_ASSERT_EQ(entities.find(e), entities.end());
        entities.insert(std::make_pair(e, vazel::ComponentSignature()));
    }
}
