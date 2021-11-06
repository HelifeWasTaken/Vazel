#include "Vazel/Components/Component.hpp"

namespace vazel
{
    void Component::remove(void)
    {
        if (_data != nullptr)
            free(_data);
        _data = nullptr;
    }

    Component::~Component(void)
    {
        remove();
    }
}
