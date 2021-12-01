/**
 * tests/Entity/test_EntityManager.cpp
 * Copyright (c) 2021 Mattis Dalleau <mattis.dalleau@epitech.eu>
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
#include "Vazel/ecs/Entity/EntityManager.hpp"

#include <gtest/gtest.h>

TEST(EntityManager, createEntity_check_if_crashes)
{
    vazel::ecs::EntityManager manager;
    vazel::ecs::Entity entity = manager.createEntity();
}

TEST(EntityManager, checkDestroyEntity_check_if_crashes)
{
    vazel::ecs::EntityManager manager;

    vazel::ecs::Entity entity = manager.createEntity();
    manager.destroyEntity(entity);
}

TEST(EntityManager, checkDestroyEntity_check_if_destroy_invalid_entity_throw)
{
    vazel::ecs::EntityManager manager;

    vazel::ecs::Entity entity = manager.createEntity();
    manager.destroyEntity(entity);
    try {
        manager.destroyEntity(entity);
        GTEST_FAIL();
    } catch (vazel::ecs::EntityManagerExceptionFindEntityError &e) {
        (void)e;
        // success
    }
}

TEST(EntityManager, checkDefaultSignature)
{
    vazel::ecs::EntityManager manager;
    vazel::ecs::Entity entity                 = manager.createEntity();
    vazel::ecs::ComponentSignature &signature = manager.getSignature(entity);
    GTEST_ASSERT_EQ(signature, vazel::ecs::ComponentSignature());
    vazel::ecs::ComponentSignature signature2 =
        vazel::ecs::ComponentSignature();
    // To make sure that the signature 2 is completely filled with zeros
    for (int i = 0; i < VAZEL_MAX_COMPONENTS; i++) {
        signature2.set(i, false);
    }
    GTEST_ASSERT_EQ(signature, signature2);
}

TEST(EntityManager, testSetSignatureByRef)
{
    vazel::ecs::EntityManager manager;
    vazel::ecs::Entity entity                 = manager.createEntity();
    vazel::ecs::ComponentSignature &signature = manager.getSignature(entity);
    signature.set(0, true);
    GTEST_ASSERT_EQ(signature, manager.getSignature(entity));
}

TEST(EntityManager, testSetSignatureByValue)
{
    vazel::ecs::EntityManager manager;
    vazel::ecs::Entity entity = manager.createEntity();
    vazel::ecs::ComponentSignature signature =
        vazel::ecs::ComponentSignature();
    signature.set(0, true);
    manager.setSignature(entity, signature);
    GTEST_ASSERT_EQ(signature, manager.getSignature(entity));
}

TEST(EntityManager,
     createEntity_check_if_multiple_entities_are_created_crashes)
{
    std::vector<vazel::ecs::Entity> entities;
    vazel::ecs::EntityManager manager;

    for (uint64_t i = 0; i < 10000; i++) {
        vazel::ecs::Entity e = manager.createEntity();
        GTEST_ASSERT_EQ(std::find(entities.begin(), entities.end(), e),
                        entities.end());
        entities.push_back(e);
    }
}

TEST(EntityManager,
     createEntity_check_if_multiple_entities_are_created_and_destroyed_crashes)
{
    std::vector<vazel::ecs::Entity> entities;
    vazel::ecs::EntityManager manager;

    for (uint64_t i = 0; i < 10000; i++) {
        entities.push_back(manager.createEntity());
    }

    for (auto entity : entities) {
        manager.destroyEntity(entity);
    }
}
