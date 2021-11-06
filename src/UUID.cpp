#include "Vazel/UUID.hpp"

namespace vazel
{
    static std::random_device s_randomDevice;
    static std::mt19937_64 s_rEngine(s_randomDevice());
    static std::uniform_int_distribution<UUID> s_uniformDistribution;

    UUID makeUUID(void)
    {
        return s_uniformDistribution(s_rEngine);
    }
}
