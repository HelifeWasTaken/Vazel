/**
 * include/Vazel/core/App.hpp
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
            std::vector<std::shared_ptr<State>> &states, uint32_t stateTag);

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
