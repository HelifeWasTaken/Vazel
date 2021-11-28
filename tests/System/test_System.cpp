#include <gtest/gtest.h>

#include "Vazel/System/System.hpp"
#include "../tests_components.hpp"

TEST(System, CreateSystem)
{
    char const *tag = "test_system";
    vazel::System system("TagName");
}

TEST(System, getTag)
{
    vazel::System system("TagName");
    ASSERT_EQ(system.getTag(), "TagName");
}

TEST(System, getSignature)
{
    vazel::System system("TagName");
    ASSERT_EQ(system.getSignature(), vazel::ComponentSignature());
}

TEST(System, addDependencies)
{
    vazel::ComponentManager cm;
    vazel::EntityManager em;
    vazel::System system("TagName");

    cm.registerComponent<placeholder_component_1>();
    cm.registerComponent<placeholder_component_2>();

    system.addDependency<placeholder_component_1>(em, cm);
    system.addDependency<placeholder_component_2>(em, cm);

    ASSERT_EQ(system.getSignature(), cm.getComponentSignature());
}

TEST(System, removeDependencies)
{
    vazel::ComponentManager cm;
    vazel::EntityManager em;
    vazel::System system("TagName");

    cm.registerComponent<placeholder_component_1>();
    cm.registerComponent<placeholder_component_2>();

    system.addDependency<placeholder_component_1>(em, cm);
    system.addDependency<placeholder_component_2>(em, cm);

    system.removeDependency<placeholder_component_1>(em, cm);

    vazel::ComponentSignature signature;
    signature.set(cm.getComponentType<placeholder_component_2>(), true);
    ASSERT_EQ(system.getSignature(), signature);
}

TEST(System, addEntities)
{
    testing::internal::CaptureStdout();

    vazel::ComponentManager cm;
    vazel::EntityManager em;
    vazel::System system("TagName");

    cm.registerComponent<placeholder_component_1>();
    cm.registerComponent<placeholder_component_2>();

    system.setOnUpdate([](vazel::ComponentManager& cm, const vazel::Entity& em) {
        std::cout << "Here is an entity!" << std::endl;
    });

    vazel::Entity entity = em.createEntity();
    vazel::Entity entity2 = em.createEntity();
    cm.attachComponent<placeholder_component_1>(entity);

    system.addDependency<placeholder_component_1>(em, cm);

    system.update(cm);
    cm.attachComponent<placeholder_component_1>(entity2);
    system.updateValidEntities(em);
    system.update(cm);

    system.addDependency<placeholder_component_2>(em, cm);
    system.update(cm);
    ASSERT_EQ(testing::internal::GetCapturedStdout(), "Here is an entity!\nHere is an entity!\nHere is an entity!\n");
}