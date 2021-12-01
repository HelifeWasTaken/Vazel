#include "../tests_components.hpp"
#include "Vazel/World/World.hpp"

#include <gtest/gtest.h>

TEST(World, CreateWorld)
{
    vazel::World world;
}

TEST(World, CreateEntity)
{
    vazel::World world;
    vazel::Entity entity = world.createEntity();
}

TEST(World, RemoveEntity)
{
    vazel::World world;
    vazel::Entity entity = world.createEntity();
    world.removeEntity(entity);
}

TEST(World, registerComponent)
{
    vazel::World world;
    vazel::Entity entity = world.createEntity();
    world.registerComponent<placeholder_component_1>();
}

TEST(World, CreateEntityWithComponent)
{
    vazel::World world;
    vazel::Entity entity = world.createEntity();

    world.registerComponent<placeholder_component_1>();
    world.registerComponent<placeholder_component_2>();
    world.attachComponent<placeholder_component_1>(entity);
}

TEST(World, CreateEntityWithComponents)
{
    vazel::World world;
    vazel::Entity entity = world.createEntity();

    world.registerComponent<placeholder_component_1>();
    world.registerComponent<placeholder_component_2>();
    world.attachComponent<placeholder_component_1>(entity);
    world.attachComponent<placeholder_component_2>(entity);
}

TEST(World, CreateEntityWithComponentsAndRemoveOne)
{
    vazel::World world;
    vazel::Entity entity = world.createEntity();

    world.registerComponent<placeholder_component_1>();
    world.registerComponent<placeholder_component_2>();

    world.attachComponent<placeholder_component_1>(entity);
    world.attachComponent<placeholder_component_2>(entity);
    world.detachComponent<placeholder_component_1>(entity);
}

TEST(World, createTwoEntities)
{
    vazel::World world;
    vazel::Entity entity  = world.createEntity();
    vazel::Entity entity2 = world.createEntity();

    EXPECT_NE(entity.getId(), entity2.getId());
}

TEST(World, createTwoEntitiesWithCompos)
{
    vazel::World world;
    vazel::Entity entity = world.createEntity();

    world.registerComponent<placeholder_component_1>();
    world.registerComponent<placeholder_component_2>();
    world.attachComponent<placeholder_component_1>(entity);
    vazel::Entity entity2 = world.createEntity();
    EXPECT_NE(entity.getId(), entity2.getId());
}

TEST(World, GeneralEntitiesWithComps)
{
    vazel::World world;
    vazel::Entity entity = world.createEntity();

    world.registerComponent<placeholder_component_1>();
    world.registerComponent<placeholder_component_2>();
    world.attachComponent<placeholder_component_1>(entity);
    world.attachComponent<placeholder_component_2>(entity);
}

TEST(World, createEntityWithComponentsAndRemoveOne)
{
    vazel::World world;
    vazel::Entity entity = world.createEntity();

    world.registerComponent<placeholder_component_1>();
    world.registerComponent<placeholder_component_2>();
    world.attachComponent<placeholder_component_1>(entity);
    world.attachComponent<placeholder_component_2>(entity);
    world.detachComponent<placeholder_component_1>(entity);
}

TEST(World, createEntityWithComponentsAndRemoveAll)
{
    vazel::World world;
    vazel::Entity entity = world.createEntity();

    world.registerComponent<placeholder_component_1>();
    world.registerComponent<placeholder_component_2>();
    world.attachComponent<placeholder_component_1>(entity);
    world.attachComponent<placeholder_component_2>(entity);
    world.detachComponent<placeholder_component_1>(entity);
    world.detachComponent<placeholder_component_2>(entity);
}

TEST(World, getComponentFromEntity)
{
    vazel::World world;
    vazel::Entity entity = world.createEntity();

    world.registerComponent<placeholder_position_component>();
    world.attachComponent<placeholder_position_component>(entity);
    placeholder_position_component& position =
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
    vazel::World world;
    vazel::System system("placeholder_system");

    world.registerComponent<placeholder_component_1>();
    system.addDependency(world.getComponentType<placeholder_component_1>());
    world.registerSystem(system);
}

TEST(World, registerSystemWithoutDependency)
{
    vazel::World world;
    vazel::System system("placeholder_system");

    try {
        world.registerSystem(system);
    } catch (vazel::WorldException& e) {
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
    vazel::World world;
    vazel::System system1("placeholder_system");
    vazel::System system2("placeholder_system_2");

    world.registerComponent<placeholder_component_1>();
    world.registerComponent<placeholder_component_2>();
    system1.addDependency(world.getComponentType<placeholder_component_1>());
    system2.addDependency(world.getComponentType<placeholder_component_2>());
    world.registerSystem(system1);
    world.registerSystem(system2);
}

TEST(World, registerSystemWithSameName)
{
    vazel::World world;
    vazel::System system("placeholder_system");

    world.registerComponent<placeholder_component_1>();
    world.registerComponent<placeholder_component_2>();
    system.addDependency(world.getComponentType<placeholder_component_1>());
    system.addDependency(world.getComponentType<placeholder_component_2>());
    world.registerSystem(system);

    try {
        vazel::System system2("placeholder_system");
        system2.addDependency(
            world.getComponentType<placeholder_component_2>());
        world.registerSystem(system2);
    } catch (vazel::WorldException& e) {
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
    vazel::World world;
    vazel::System system("placeholder_system");

    world.registerComponent<placeholder_component_1>();
    system.addDependency(world.getComponentType<placeholder_component_1>());
    world.registerSystem(system);
    world.removeSystem("placeholder_system");
}

TEST(World, removeASystemDependency)
{
    vazel::World world;
    vazel::System system("placeholder_system");

    world.registerComponent<placeholder_component_1>();
    system.addDependency(world.getComponentType<placeholder_component_1>());
    world.registerSystem(system);
    world.removeSystemDependency<placeholder_component_1>("placeholder_system");
}

TEST(World, removeASystemDependencyNonAttached)
{
    vazel::World world;
    vazel::System system("placeholder_system");

    world.registerComponent<placeholder_component_1>();
    system.addDependency(world.getComponentType<placeholder_component_1>());
    world.registerSystem(system);
    try {
        world.removeSystemDependency<placeholder_component_2>(
            "placeholder_system");
    } catch (vazel::WorldException& e) {
        return;
    } catch (...) {
        std::cerr << "Unexpected exception" << std::endl;
        GTEST_FAIL();
    }
    std::cerr << "No exception thrown" << std::endl;
    GTEST_FAIL();
}

/*
TEST(World, updateSystem)
{

}
*/