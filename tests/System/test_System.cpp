/**
 * tests/System/test_System.cpp
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
#include "../tests_components.hpp"
#include "Vazel/ecs/System/System.hpp"

#include <gtest/gtest.h>

TEST(System, CreateSystem)
{
    char const *tag = "test_system";
    vazel::ecs::System system("TagName");
}

TEST(System, getTag)
{
    vazel::ecs::System system("TagName");
    ASSERT_EQ(system.getTag(), "TagName");
}

TEST(System, getSignature)
{
    vazel::ecs::System system("TagName");
    ASSERT_EQ(system.getSignature(), vazel::ecs::ComponentSignature());
}

TEST(System, addDependencies)
{
    vazel::ecs::ComponentManager cm;
    vazel::ecs::EntityManager em;
    vazel::ecs::System system("TagName");

    cm.registerComponent<placeholder_component_1>();
    cm.registerComponent<placeholder_component_2>();

    system.addDependency<placeholder_component_1>(em, cm);
    system.addDependency<placeholder_component_2>(em, cm);

    ASSERT_EQ(system.getSignature(), cm.getComponentSignature());
}

TEST(System, removeDependencies)
{
    vazel::ecs::ComponentManager cm;
    vazel::ecs::EntityManager em;
    vazel::ecs::System system("TagName");

    cm.registerComponent<placeholder_component_1>();
    cm.registerComponent<placeholder_component_2>();

    system.addDependency<placeholder_component_1>(em, cm);
    system.addDependency<placeholder_component_2>(em, cm);

    system.removeDependency<placeholder_component_1>(em, cm);

    vazel::ecs::ComponentSignature signature;
    signature.set(cm.getComponentType<placeholder_component_2>(), true);
    ASSERT_EQ(system.getSignature(), signature);
}

TEST(System, addEntities)
{
    testing::internal::CaptureStdout();

    vazel::ecs::ComponentManager cm;
    vazel::ecs::EntityManager em;
    vazel::ecs::System system("TagName");

    cm.registerComponent<placeholder_component_1>();
    cm.registerComponent<placeholder_component_2>();

    system.setOnUpdate(VAZEL_SYSTEM_UPDATE_LAMBDA(cm, em) {
        std::cout << "Here is an entity!";
    });

    vazel::ecs::Entity entity  = em.createEntity();
    vazel::ecs::Entity entity2 = em.createEntity();

    cm.onEntityCreate(entity);
    cm.attachComponent<placeholder_component_1>(entity);
    cm.onEntityCreate(entity2);
    em.getSignature(entity).set(cm.getComponentType<placeholder_component_1>(),
                                true);

    system.addDependency<placeholder_component_1>(em, cm);
    system.onUpdate(cm);

    cm.attachComponent<placeholder_component_1>(entity2);
    em.getSignature(entity2).set(
        cm.getComponentType<placeholder_component_1>(), true);
    system.updateValidEntities(em);
    system.onUpdate(cm);

    system.addDependency<placeholder_component_2>(em, cm);
    system.onUpdate(cm);
    ASSERT_EQ(testing::internal::GetCapturedStdout(),
              "Here is an entity!Here is an entity!Here is an entity!");
}
