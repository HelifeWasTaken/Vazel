/**
 * include/Vazel/core/State.hpp
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

#include "Vazel/core/App.hpp"

#include <cstdint>
#include <functional>
#include <memory>

namespace vazel
{
    namespace core
    {

        class App;

        class State
        {
          private:
            std::function<void(App &)> _on_init;
            std::function<bool(App &)> _on_update;
            std::function<void(App &)> _on_exit;
            uint32_t _tag;
            bool _is_running = false;

          public:
            State(std::function<void(App &)> on_init,
                  std::function<bool(App &)> on_update,
                  std::function<void(App &)> on_exit, uint32_t tag);

            ~State(void) = default;

            const bool isRunning(void);

            const uint32_t &getTag(void);

            void init(App &app);

            void update(App &app);

            void exit(App &app);
        };

        const std::vector<std::shared_ptr<State>>::iterator getStateFromTag(
            std::vector<std::shared_ptr<State>> &states, uint32_t stateTag);

    } // namespace core
} // namespace vazel
