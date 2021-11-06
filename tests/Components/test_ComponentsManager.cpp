#include <gtest/gtest.h>
#include "Vazel/Components/ComponentsManager.hpp"

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
        std::cout << "Invalid: Exception catched was not vazel::ComponentManagerRegisterError" << std::endl;
        GTEST_FAIL();
    }
    std::cout << "Invalid: No exception was catched" << std::endl;
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
        std::cout << "Invalid: Exception catched was not vazel::ComponentManagerFindComponentError" << std::endl;
        GTEST_FAIL();
    }
    std::cout << "Invalid: No exception was catched" << std::endl;
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
        std::cout << "Invalid: Exception catched was not vazel::ComponentManagerFindComponentError" << std::endl;
        GTEST_FAIL();
    }
    std::cout << "Invalid: No exception was catched" << std::endl;
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
