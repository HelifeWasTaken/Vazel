#include <gtest/gtest.h>
#include "Vazel/Components/ComponentsManager.hpp"

struct placeholder_component_1 {

};

TEST(ComponentGeneration, TestOneComponentRegister)
{
    vazel::ComponentManager cm;

    cm.registerComponent<placeholder_component_1>();
}

TEST(ComponentGeneration, TestGetComponentRegistered)
{
    vazel::ComponentManager cm;

    cm.registerComponent<placeholder_component_1>();
//    GTEST_ASSERT_EQ(cm.getComponentType<placeholder_component_1>(), 0);
}
