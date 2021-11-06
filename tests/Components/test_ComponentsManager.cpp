#include <gtest/gtest.h>
#include "Vazel/Components/ComponentsManager.hpp"
#include "Vazel/UUID.hpp"

struct placeholder_component_1 {};

struct placeholder_component_2 {};

struct placeholder_component_3 {};

struct placeholder_component_4 {};

struct placeholder_component_5 {};

struct placeholder_component_6 {};

struct placeholder_component_7 {};

TEST(ComponentRegistering, TestOneComponentRegister)
{
    vazel::ComponentManager cm;

    cm.Register<placeholder_component_1>();
}

TEST(ComponentRegistering, TestGetComponentRegistered)
{
    vazel::ComponentManager cm;

    cm.Register<placeholder_component_1>();
    GTEST_ASSERT_EQ(cm.getComponentType<placeholder_component_1>(), 0);
}

TEST(ComponentRegistering, TwoComponents)
{
    vazel::ComponentManager cm;

    cm.Register<placeholder_component_1>();
    cm.Register<placeholder_component_2>();
    GTEST_ASSERT_EQ(cm.getComponentType<placeholder_component_1>(), 0);
    GTEST_ASSERT_EQ(cm.getComponentType<placeholder_component_2>(), 1);
}

TEST(ComponentRegistering, RaiseExceptionWhenComponentRegisteredTwice)
{
    vazel::ComponentManager cm;

    cm.Register<placeholder_component_1>();
    try {
        cm.Register<placeholder_component_1>();
    } catch (vazel::ComponentManagerException& e) {
        return;
    } catch (std::exception& e) {
        std::cerr << "Invalid: Exception catched was not vazel::ComponentManagerRegisterError" << std::endl;
        GTEST_FAIL();
    }
    std::cerr << "Invalid: No exception was catched" << std::endl;
    GTEST_FAIL();
}

TEST(ComponentRegistering, RaiseExceptionWhenGettingNonExistingComponent)
{
    vazel::ComponentManager cm;

    try {
        cm.getComponentType<placeholder_component_1>();
    } catch (vazel::ComponentManagerException& e) {
        return;
    } catch (std::exception& e) {
        std::cerr << "Invalid: Exception catched was not vazel::ComponentManagerFindComponentError" << std::endl;
        GTEST_FAIL();
    }
    std::cerr << "Invalid: No exception was catched" << std::endl;
    GTEST_FAIL();
}

TEST(ComponentRegistering, removeExistingSingleComponent)
{
    vazel::ComponentManager cm;

    cm.Register<placeholder_component_1>();
    GTEST_ASSERT_EQ(cm.getComponentType<placeholder_component_1>(), 0);
    cm.unRegister<placeholder_component_1>();
    try {
        cm.getComponentType<placeholder_component_1>();
    } catch (vazel::ComponentManagerRegisterError& e) {
        return;
    } catch (std::exception& e) {
        std::cerr << "Invalid: Exception catched was not vazel::ComponentManagerFindComponentError" << std::endl;
        GTEST_FAIL();
    }
    std::cerr << "Invalid: No exception was catched" << std::endl;
    GTEST_FAIL();
}

TEST(ComponentRegistering, removeOneComponentAndAddAnotherOneToTakeSlotZero)
{
    vazel::ComponentManager cm;

    cm.Register<placeholder_component_1>();
    cm.unRegister<placeholder_component_1>();
    cm.Register<placeholder_component_2>();
    GTEST_ASSERT_EQ(cm.getComponentType<placeholder_component_2>(), 0);
}

TEST(ComponentRegistering, removeComponentsAndAddSome)
{
    vazel::ComponentManager cm;

    cm.Register<placeholder_component_1>();
    cm.Register<placeholder_component_2>();
    cm.unRegister<placeholder_component_1>();
    cm.Register<placeholder_component_3>();
    GTEST_ASSERT_EQ(cm.getComponentType<placeholder_component_2>(), 1);
    GTEST_ASSERT_EQ(cm.getComponentType<placeholder_component_3>(), 0);
}

const static std::string LOGEXPECT =
"----- Components Begin -----\n"
"Name: [23placeholder_component_1] Id: [0]\n"
"------ Components End ------\n";

vazel::ComponentManager _testLogginBasicLaunch()
{
    testing::internal::CaptureStdout();
    vazel::ComponentManager cm;
    cm.Register<placeholder_component_1>();

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

struct placeholder_position_component {
    float x;
    float y;
};
TEST(ComponentUsage, registerPositionComponentAndChangeIt)
{
    vazel::ComponentManager cm;
    vazel::Entity e;

    cm.Register<placeholder_position_component>();
    cm.onEntityCreate(e);
    cm.attachComponent<placeholder_position_component>(e);
    auto& p = cm.getComponent<placeholder_position_component>(e);
    p.x = 6;

    GTEST_ASSERT_EQ(cm.getComponent<placeholder_position_component>(e).x, 6);
    GTEST_ASSERT_EQ(cm.getComponent<placeholder_position_component>(e).y, 0);
}

struct entity_offsetx_offsety {
    float ofx;
    float ofy;
};
TEST(ComponentUsage, registerPositionMultiplesEntities)
{
    std::map<vazel::Entity, entity_offsetx_offsety> s;
    vazel::ComponentManager cm;

    cm.Register<entity_offsetx_offsety>();
    for (size_t i = 0; i != 10000; i++) {
        auto id = vazel::makeUUID();
        entity_offsetx_offsety tmp;
        vazel::Entity e;
        tmp.ofx = id;
        tmp.ofy = id;
        cm.onEntityCreate(e);
        cm.attachComponent<entity_offsetx_offsety>(e);
        auto& comp = cm.getComponent<entity_offsetx_offsety>(e);
        comp.ofx = id + 3;
        comp.ofy = id + 2;
        s.emplace(e, tmp);
    }
    for (auto it : s) {
        auto& comp = cm.getComponent<entity_offsetx_offsety>(it.first);
        GTEST_ASSERT_EQ(it.second.ofx, comp.ofx - 3);
        GTEST_ASSERT_EQ(it.second.ofy, comp.ofy - 2);
    }
}

struct placeholder_string_component {
    std::string s;
};
TEST(ComponentUsage, registerTwoDifferentComponents)
{
    vazel::ComponentManager cm;

    cm.Register<entity_offsetx_offsety>();
    cm.Register<placeholder_string_component>();
    vazel::Entity e;
    cm.onEntityCreate(e);
    cm.attachComponent<placeholder_string_component>(e);
    cm.attachComponent<entity_offsetx_offsety>(e);
    cm.getComponent<placeholder_string_component>(e).s += "lol";
    cm.getComponent<entity_offsetx_offsety>(e).ofx = 4;
    cm.getComponent<entity_offsetx_offsety>(e).ofy = 3;
    GTEST_ASSERT_EQ(cm.getComponent<placeholder_string_component>(e).s, std::string("lol"));
    entity_offsetx_offsety off = { 4, 3 };
    GTEST_ASSERT_EQ(cm.getComponent<entity_offsetx_offsety>(e).ofx, off.ofx);
    GTEST_ASSERT_EQ(cm.getComponent<entity_offsetx_offsety>(e).ofy, off.ofy);
}

TEST(ComponentUsage, getComponentWithoutRegisteredEntity)
{
    vazel::ComponentManager cm;
    vazel::Entity unregisteredEntity;

    cm.Register<placeholder_component_1>();
    try {
        cm.getComponent<placeholder_component_1>(unregisteredEntity);
    } catch (vazel::ComponentManagerException& e) {
        return;
    } catch (...) {
        std::cerr << "Error: Got Invalid type of exception" << std::endl;
        GTEST_FAIL();
    }
    std::cerr << "Error: Got no exception" << std::endl;
    GTEST_FAIL();
}
