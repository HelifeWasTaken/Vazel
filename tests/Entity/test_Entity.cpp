#include <gtest/gtest.h>
#include <Vazel/Entity/Entity.hpp>
#include <unordered_set>

TEST(EntitiesGeneration, GenerateOneEntity)
{
    std::cout << "EntityRandomId: " << vazel::Entity().getId() << std::endl;
}

TEST(EntitiesGeneration, MakeSureThereIsNoCollisions)
{
    std::unordered_set<uint64_t> _set;

    for (size_t i = 0; i != 1000; i++) {
        vazel::Entity e;
        uint64_t res = std::hash<vazel::Entity>()(e);
        GTEST_ASSERT_EQ(_set.find(res), _set.end());
        _set.emplace(res);
    }
}
