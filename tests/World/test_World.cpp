/**
 * World/test_World.cpp
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
#include "../tests_components.hpp"
#include "Vazel/ecs/World/World.hpp"

#include <gtest/gtest.h>

TEST(World, CreateWorld)
{
    vazel::ecs::World world;
}

TEST(World, CreateEntity)
{
    vazel::ecs::World world;
    vazel::ecs::Entity entity = world.createEntity();
}

TEST(World, RemoveEntity)
{
    vazel::ecs::World world;
    vazel::ecs::Entity entity = world.createEntity();
    world.removeEntity(entity);
}

TEST(World, registerComponent)
{
    vazel::ecs::World world;
    vazel::ecs::Entity entity = world.createEntity();
    world.registerComponent<placeholder_component_1>();
}

TEST(World, CreateEntityWithComponent)
{
    vazel::ecs::World world;
    vazel::ecs::Entity entity = world.createEntity();

    world.registerComponent<placeholder_component_1>();
    world.registerComponent<placeholder_component_2>();
    world.attachComponent<placeholder_component_1>(entity);
}

TEST(World, CreateEntityWithComponents)
{
    vazel::ecs::World world;
    vazel::ecs::Entity entity = world.createEntity();

    world.registerComponent<placeholder_component_1>();
    world.registerComponent<placeholder_component_2>();
    world.attachComponent<placeholder_component_1>(entity);
    world.attachComponent<placeholder_component_2>(entity);
}

TEST(World, CreateEntityWithComponentsAndRemoveOne)
{
    vazel::ecs::World world;
    vazel::ecs::Entity entity = world.createEntity();

    world.registerComponent<placeholder_component_1>();
    world.registerComponent<placeholder_component_2>();

    world.attachComponent<placeholder_component_1>(entity);
    world.attachComponent<placeholder_component_2>(entity);
    world.detachComponent<placeholder_component_1>(entity);
}

TEST(World, createTwoEntities)
{
    vazel::ecs::World world;
    vazel::ecs::Entity entity  = world.createEntity();
    vazel::ecs::Entity entity2 = world.createEntity();

    EXPECT_NE(entity.getId(), entity2.getId());
}

TEST(World, createTwoEntitiesWithCompos)
{
    vazel::ecs::World world;
    vazel::ecs::Entity entity = world.createEntity();

    world.registerComponent<placeholder_component_1>();
    world.registerComponent<placeholder_component_2>();
    world.attachComponent<placeholder_component_1>(entity);
    vazel::ecs::Entity entity2 = world.createEntity();
    EXPECT_NE(entity.getId(), entity2.getId());
}

TEST(World, GeneralEntitiesWithComps)
{
    vazel::ecs::World world;
    vazel::ecs::Entity entity = world.createEntity();

    world.registerComponent<placeholder_component_1>();
    world.registerComponent<placeholder_component_2>();
    world.attachComponent<placeholder_component_1>(entity);
    world.attachComponent<placeholder_component_2>(entity);
}

TEST(World, createEntityWithComponentsAndRemoveOne)
{
    vazel::ecs::World world;
    vazel::ecs::Entity entity = world.createEntity();

    world.registerComponent<placeholder_component_1>();
    world.registerComponent<placeholder_component_2>();
    world.attachComponent<placeholder_component_1>(entity);
    world.attachComponent<placeholder_component_2>(entity);
    world.detachComponent<placeholder_component_1>(entity);
}

TEST(World, createEntityWithComponentsAndRemoveAll)
{
    vazel::ecs::World world;
    vazel::ecs::Entity entity = world.createEntity();

    world.registerComponent<placeholder_component_1>();
    world.registerComponent<placeholder_component_2>();
    world.attachComponent<placeholder_component_1>(entity);
    world.attachComponent<placeholder_component_2>(entity);
    world.detachComponent<placeholder_component_1>(entity);
    world.detachComponent<placeholder_component_2>(entity);
}

TEST(World, getComponentFromEntity)
{
    vazel::ecs::World world;
    vazel::ecs::Entity entity = world.createEntity();

    world.registerComponent<placeholder_position_component>();
    world.attachComponent<placeholder_position_component>(entity);
    placeholder_position_component &position =
        world.getComponent<placeholder_position_component>(entity);

    position.x = 59;
    EXPECT_EQ(position.x,
              world.getComponent<placeholder_position_component>(entity).x);
    EXPECT_EQ(position.y,
              world.getComponent<placeholder_position_component>(entity).y);
    EXPECT_EQ(&position,
              &world.getComponent<placeholder_position_component>(entity));
}

TEST(World, registerSystem)
{
    vazel::ecs::World world;
    vazel::ecs::System system("placeholder_system");

    world.registerComponent<placeholder_component_1>();
    system.addDependency(world.getComponentType<placeholder_component_1>());
    world.registerSystem(system);
}

TEST(World, registerSystemWithoutDependency)
{
    vazel::ecs::World world;
    vazel::ecs::System system("placeholder_system");

    try {
        world.registerSystem(system);
    } catch (vazel::ecs::WorldException &e) {
        return;
    } catch (...) {
        std::cerr << "Unexpected exception" << std::endl;
        GTEST_FAIL();
    }
    std::cerr << "No exception thrown" << std::endl;
    GTEST_FAIL();
}

TEST(World, registerMultipleSystems)
{
    vazel::ecs::World world;
    vazel::ecs::System system1("placeholder_system");
    vazel::ecs::System system2("placeholder_system_2");

    world.registerComponent<placeholder_component_1>();
    world.registerComponent<placeholder_component_2>();
    system1.addDependency(world.getComponentType<placeholder_component_1>());
    system2.addDependency(world.getComponentType<placeholder_component_2>());
    world.registerSystem(system1);
    world.registerSystem(system2);
}

TEST(World, registerSystemWithSameName)
{
    vazel::ecs::World world;
    vazel::ecs::System system("placeholder_system");

    world.registerComponent<placeholder_component_1>();
    world.registerComponent<placeholder_component_2>();
    system.addDependency(world.getComponentType<placeholder_component_1>());
    system.addDependency(world.getComponentType<placeholder_component_2>());
    world.registerSystem(system);

    try {
        vazel::ecs::System system2("placeholder_system");
        system2.addDependency(
            world.getComponentType<placeholder_component_2>());
        world.registerSystem(system2);
    } catch (vazel::ecs::WorldException &e) {
        return;
    } catch (...) {
        std::cerr << "Unexpected exception" << std::endl;
        GTEST_FAIL();
    }
    std::cerr << "No exception thrown" << std::endl;
    GTEST_FAIL();
}

TEST(World, removeASystem)
{
    vazel::ecs::World world;
    vazel::ecs::System system("placeholder_system");

    world.registerComponent<placeholder_component_1>();
    system.addDependency(world.getComponentType<placeholder_component_1>());
    world.registerSystem(system);
    world.removeSystem("placeholder_system");
}

/*
TEST(World, updateSystem)
{

}
*/
