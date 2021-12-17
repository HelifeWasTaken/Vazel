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

#include "Vazel/core/Renderer/Renderer.hpp"
#include "Vazel/core/State/State.hpp"
#include "Vazel/ecs/World/World.hpp"

namespace vazel
{
    namespace core
    {
        /**
         * @brief AppException base class
         *
         */
        class AppException : public VException
        {
          private:
            std::string _e;

          public:
            /**
             * @brief Construct a new App Exception Object
             *
             * @param e The error message.
             */
            AppException(const std::string &e);

            /**
             * @brief Get the what object.
             *
             * @return const char* The error message.
             */
            const char *what() const throw() override;
        };

        /**
         * @brief State forward declaration
         */
        class State;

        /**
         * @brief Get the State From Tag object (Used when forward declaration
         * gives a warning) And you can't directly use find on the State object
         * inside App.
         * @param states The states list.
         * @param stateTag The state tag to find.
         * @return const std::vector<std::shared_ptr<State>>::iterator The
         * state iterator.
         */
        const std::vector<std::shared_ptr<State>>::iterator getStateFromTag(
            std::vector<std::shared_ptr<State>> &states, uint32_t stateTag);

        class App
        {
          private:
            std::vector<std::shared_ptr<State>> _states;
            State *_current_state = nullptr;
            State *_pending_state = nullptr;

          public:
            Renderer renderer;
            ecs::World world;
            sf::RenderWindow window;

            /**
             * @brief Construct a new App object
             *
             */
            App(State &baseState);

            /**
             * @brief Destroy the App object
             *
             */
            ~App(void) = default;

            /**
             * @brief Stop the current state.
             *
             */
            void stop(void);

            /**
             * @brief Set the current state.
             *
             * @param stateTag The state tag.
             */
            void setState(uint32_t stateTag);

            /**
             * @brief Register a new state.
             *
             * @param state The state to register.
             */
            void registerState(State &state);

            /**
             * @brief Init the pending state and set it as the current state.
             *        Then call the update state while it's running.
             */
            void run(void);
        };

        extern App *g_app;

    } // namespace core
} // namespace vazel
