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

    ComponentExistsException::ComponentExistsException(const std::string& e)
        : _e(e) {}

    const char* ComponentExistsException::what(void) const noexcept
    {
        return _e.c_str();
    }
}
