# Vazel

A better attempt to do an real and portable and not heritage based Entity Component System

## Dependencies

    - make
    - cmake
    - g++
    - gcc
    - gtest (Tests)

```cpp
// I could technically have just used directly std::string
struct stringComponent {
    std::string s;
};

struct positionComponent {
    float x;
    float y;
};

std::ostream& operator<<(std::ostream& os, const positionComponent& pos)
{
    os << "x: [" << pos.x << "] y: [" << pos.y << "]";
    return os;
}

// There is no vazel::system yet so it is still rough to use for now

void modifyStuff(const vazel::Entity& e, vazel::ComponentManager& cm)
{
    // Register the differents components that the entities can use
    cm.Register<positionComponent>();
    cm.Register<stringComponent>();

    // Register the current entity
    // (it should not be done by hand but there is no vazel::system implentation
    // that does that automatically that yet)
    cm.onEntityCreate(e);

    // Attach the differents components to the current Entity
    cm.attachComponent<stringComponent>(e);
    cm.attachComponent<positionComponent>(e);
    // It will be soon possible to give also a base value like so:
    // cm.attachComponent<positionComponent>(e,
    //    PositionComponent {
    //      .x = 30,
    //      .y = 20
    //    }
    // );

    // Get the differents components
    // It is a ref to a void * allocated with calloc
    auto& componentStr = cm.getComponent<stringComponent>(e);
    auto& comPos = cm.getComponent<positionComponent>(e)

    // Modify the values by reference
    componentStr.s = "lol";
    comPos.ofx = 4;
    comPos.ofy = 3;
}

int main(void)
{
    vazel::ComponentManager cm;
    vazel::Entity e;


    // See the current Entity Id (UUID)
    std::cout << "EntityId: " << e.getId() << std::endl;

    // This may works some times implictly without needing to specify the cast
    // It will transform itself in vazel::UUID a.k.a uint64_t
    std::cout << "Getting EntityId works also with a cast: " << (vazel::UUID)e << std::endl;

    // Initialize stuff and change values by ref (see upper)
    modifyStuff(e, cm);

    // See te result of modifyStuff and get to see if the modification by ref works well
    std::cout << "StrComponent: " << cm.getComponent<stringComponent>(e).s
            << "PosComponent: " << cm.getComponent<positionComponent>(e)
            << std::endl;
}
```
