#pragma once

#include "Vazel/core/State.hpp"
#include "Vazel/ecs.hpp"

namespace vazel
{
    using World = ecs::World;

    namespace core
    {

        class State;
        const std::vector<std::shared_ptr<State>>::iterator getStateFromTag(
            std::vector<std::shared_ptr<State>> states, uint32_t stateTag);

        class App
        {
          private:
            World _world;
            std::vector<std::shared_ptr<State>> _states;
            State *_current_state = nullptr;
            State *_pending_state = nullptr;

          public:
            App(State &baseState);

            ~App(void) = default;

            void stop(void);

            void setState(uint32_t stateTag);

            void registerState(State &state);

            World &getWorld(void);

            void run(void);
        };

    } // namespace core
} // namespace vazel
