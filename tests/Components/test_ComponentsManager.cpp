/**
 * tests/Components/test_ComponentsManager.cpp
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
#include "Vazel/UUID.hpp"
#include "Vazel/ecs/Components/ComponentsManager.hpp"

#include <gtest/gtest.h>

TEST(ComponentRegistering, TestOneComponentRegister)
{
    vazel::ecs::ComponentManager cm;

    cm.registerComponent<placeholder_component_1>();
}

TEST(ComponentRegistering, TestGetComponentRegistered)
{
    vazel::ecs::ComponentManager cm;

    cm.registerComponent<placeholder_component_1>();
    GTEST_ASSERT_EQ(cm.getComponentType<placeholder_component_1>(), 0);
}

TEST(ComponentRegistering, TwoComponents)
{
    vazel::ecs::ComponentManager cm;

    cm.registerComponent<placeholder_component_1>();
    cm.registerComponent<placeholder_component_2>();
    GTEST_ASSERT_EQ(cm.getComponentType<placeholder_component_1>(), 0);
    GTEST_ASSERT_EQ(cm.getComponentType<placeholder_component_2>(), 1);
}

TEST(ComponentRegistering,
     SameComponentTypeWhenRegisteringTwiceTheSameComponent)
{
    vazel::ecs::ComponentManager cm;

    vazel::ecs::ComponentType t =
        cm.registerComponent<placeholder_component_1>();
    GTEST_ASSERT_EQ(t, cm.registerComponent<placeholder_component_1>());
}

TEST(ComponentRegistering, RaiseExceptionWhenGettingNonExistingComponent)
{
    vazel::ecs::ComponentManager cm;

    try {
        cm.getComponentType<placeholder_component_1>();
    } catch (vazel::ecs::ComponentManagerException &e) {
        return;
    } catch (std::exception &e) {
        std::cerr << "Invalid: Exception catched was not "
                     "vazel::ecs::ComponentManagerFindComponentError"
                  << std::endl;
        GTEST_FAIL();
    }
    std::cerr << "Invalid: No exception was catched" << std::endl;
    GTEST_FAIL();
}

TEST(ComponentRegistering, removeExistingSingleComponent)
{
    vazel::ecs::ComponentManager cm;

    cm.registerComponent<placeholder_component_1>();
    GTEST_ASSERT_EQ(cm.getComponentType<placeholder_component_1>(), 0);
    cm.unregisterComponent<placeholder_component_1>();
    try {
        cm.getComponentType<placeholder_component_1>();
    } catch (vazel::ecs::ComponentManagerRegisterError &e) {
        return;
    } catch (std::exception &e) {
        std::cerr << "Invalid: Exception catched was not "
                     "vazel::ecs::ComponentManagerFindComponentError"
                  << std::endl;
        GTEST_FAIL();
    }
    std::cerr << "Invalid: No exception was catched" << std::endl;
    GTEST_FAIL();
}

TEST(ComponentRegistering, removeOneComponentAndAddAnotherOneToTakeSlotZero)
{
    vazel::ecs::ComponentManager cm;

    cm.registerComponent<placeholder_component_1>();
    cm.unregisterComponent<placeholder_component_1>();
    cm.registerComponent<placeholder_component_2>();
    GTEST_ASSERT_EQ(cm.getComponentType<placeholder_component_2>(), 0);
}

TEST(ComponentRegistering, removeComponentsAndAddSome)
{
    vazel::ecs::ComponentManager cm;

    cm.registerComponent<placeholder_component_1>();
    cm.registerComponent<placeholder_component_2>();
    cm.unregisterComponent<placeholder_component_1>();
    cm.registerComponent<placeholder_component_3>();
    GTEST_ASSERT_EQ(cm.getComponentType<placeholder_component_2>(), 1);
    GTEST_ASSERT_EQ(cm.getComponentType<placeholder_component_3>(), 0);
}

const static std::string LOGEXPECT =
    "----- Components Begin -----\n"
    "Name: [23placeholder_component_1] Id: [0]\n"
    "------ Components End ------\n";

vazel::ecs::ComponentManager _testLogginBasicLaunch()
{
    testing::internal::CaptureStdout();
    vazel::ecs::ComponentManager cm;
    cm.registerComponent<placeholder_component_1>();

    return cm;
}

TEST(ComponentRegistering, TestLoggingBasic)
{
    auto cm = _testLogginBasicLaunch();
    cm.showState();
    std::string res = testing::internal::GetCapturedStdout();
    GTEST_ASSERT_EQ(res, LOGEXPECT);
}

TEST(ComponentRegistering, TestLoggingBasicWithBinaryOperatorOstream)
{
    auto cm = _testLogginBasicLaunch();

    std::cout << cm;
    std::string res = testing::internal::GetCapturedStdout();
    GTEST_ASSERT_EQ(res, LOGEXPECT);
}

TEST(ComponentUsage, registerPositionComponentAndChangeIt)
{
    vazel::ecs::ComponentManager cm;
    vazel::ecs::Entity e;

    cm.registerComponent<placeholder_position_component>();
    cm.onEntityCreate(e);
    cm.attachComponent<placeholder_position_component>(e);
    auto &p = cm.getComponent<placeholder_position_component>(e);
    p.x     = 6;

    GTEST_ASSERT_EQ(cm.getComponent<placeholder_position_component>(e).x, 6);
    GTEST_ASSERT_EQ(cm.getComponent<placeholder_position_component>(e).y, 0);
}

TEST(ComponentUsage, registerPositionMultiplesEntities)
{
    std::map<vazel::ecs::Entity, entity_offsetx_offsety> s;
    vazel::ecs::ComponentManager cm;

    cm.registerComponent<entity_offsetx_offsety>();
    for (size_t i = 0; i != 10000; i++) {
        auto id = vazel::makeUUID();
        entity_offsetx_offsety tmp;
        vazel::ecs::Entity e;
        tmp.ofx = id;
        tmp.ofy = id;
        cm.onEntityCreate(e);
        cm.attachComponent<entity_offsetx_offsety>(e);
        auto &comp = cm.getComponent<entity_offsetx_offsety>(e);
        comp.ofx   = id + 3;
        comp.ofy   = id + 2;
        s.emplace(e, tmp);
    }
    for (auto it : s) {
        auto &comp = cm.getComponent<entity_offsetx_offsety>(it.first);
        GTEST_ASSERT_EQ(it.second.ofx, comp.ofx - 3);
        GTEST_ASSERT_EQ(it.second.ofy, comp.ofy - 2);
    }
}

TEST(ComponentUsage, registerTwoDifferentComponents)
{
    vazel::ecs::ComponentManager cm;

    cm.registerComponent<entity_offsetx_offsety>();
    cm.registerComponent<std::string>();
    vazel::ecs::Entity e;
    cm.onEntityCreate(e);
    cm.attachComponent<std::string>(e);
    cm.attachComponent<entity_offsetx_offsety>(e);
    cm.getComponent<std::string>(e)                = "lol";
    cm.getComponent<entity_offsetx_offsety>(e).ofx = 4;
    cm.getComponent<entity_offsetx_offsety>(e).ofy = 3;
    GTEST_ASSERT_EQ(cm.getComponent<std::string>(e), std::string("lol"));
    entity_offsetx_offsety off = { 4, 3 };
    GTEST_ASSERT_EQ(cm.getComponent<entity_offsetx_offsety>(e).ofx, off.ofx);
    GTEST_ASSERT_EQ(cm.getComponent<entity_offsetx_offsety>(e).ofy, off.ofy);
}

TEST(ComponentUsage, getComponentWithoutRegisteredEntity)
{
    vazel::ecs::ComponentManager cm;
    vazel::ecs::Entity unregisteredEntity;

    cm.registerComponent<placeholder_component_1>();
    try {
        cm.getComponent<placeholder_component_1>(unregisteredEntity);
    } catch (vazel::ecs::ComponentManagerException &e) {
        return;
    } catch (...) {
        std::cerr << "Error: Got Invalid type of exception" << std::endl;
        GTEST_FAIL();
    }
    std::cerr << "Error: Got no exception" << std::endl;
    GTEST_FAIL();
}
