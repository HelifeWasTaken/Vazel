# Vazel

A better attempt to do an real and portable and not heritage based Entity Component System

## Dependencies

    - make
    - cmake
    - g++
    - gcc
    - gtest (Tests)

## Example:

Here is an example of how you can create a World that contains a System taking each Entity with a `Vector2` and `Vector3` Component and update it.


```cpp
struct Vector2 {
    int x, y;
};

struct Vector3 {
    int x, y, z;
};

std::ostream &operator<<(std::ostream &os, Vector2 &v2)
{
    return os << "Vector2(x: " << v2.x << ", y: " << v2.y << ")";
}

std::ostream &operator<<(std::ostream &os, Vector3 &v3)
{
    return os << "Vector3(x: " << v3.x << ", y: " << v3.y << ", z: " << v3.z);
}

int main(void)
{
    vazel::World world;
    vazel::System system("ExampleSystem");
    vazel::Entity e = world.createEntity();

    vazel::ComponentType vec2Type = world.registerComponent<Vector2>();
    vazel::ComponentTYpe vec3Type = world.registerComponent<Vector3>();

    world.attachComponent<Vector2>(e);
    world.attachComponent<Vector3>(e);

    system.addDependency(vec2Type);
    system.addDependency(vec3Type);

    system.setOnUpdate(
        VAZEL_SYSTEM_UPDATE_LAMBDA() {
            // See te result of modifyStuff and get to see if the modification by ref
            // works well
            auto &v2 = cm.getComponent<Vector2>(e);
            auto &v3 = cm.getComponent<Vector3>(e);

            std::cout << "v2: " << v2 << " " <<
                  << "v3: " << v3 << " " <<
                  << std::endl;
            v2.x += 1;
            v2.y += 1;
            v3.x += 1;
            v3.y += 1;
            v3.z += 1;
        }
    );

    world.registerSystem(system);
    while (true)
        world.updateSystem();
}
```

## LICENSE
```
 README.md
 Copyright (c) 2021 Mattis DALLEAU <mattisdalleau@gmail.com>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 (see LICENSE.md for the complete license)
```

