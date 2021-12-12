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

#include "Vazel/VException.hpp"
#include "Vazel/core/App/App.hpp"

#include <cstdint>
#include <functional>
#include <memory>

namespace vazel
{
    namespace core
    {

        /**
         * @brief StateException base class
         *
         */
        class StateException : public VException
        {
          private:
            std::string _e;

          public:
            /**
             * @brief Construct a new State Exception Object
             *
             * @param e The error message
             *
             */
            StateException(const std::string &e);

            /**
             * @brief Get the what object.
             *
             * @return const char* The error message
             */
            const char *what() const throw() override;
        };

        /**
         * @brief App forwared declaration
         */
        class App;

        /**
         * @brief State base class
         *
         */
        class State
        {
          private:
            std::function<void(App &)> _on_init;
            std::function<bool(App &)> _on_update;
            std::function<void(App &)> _on_exit;
            uint32_t _tag;
            bool _is_running = false;

          public:
            /**
             * @brief Construct a new State Object
             *
             * @param tag The state tag
             * @param on_init The init function
             * @param on_update The update function
             * @param on_exit The exit function
             */
            State(std::function<void(App &)> on_init,
                  std::function<bool(App &)> on_update,
                  std::function<void(App &)> on_exit, uint32_t tag);

            /**
             * @brief Destroy the State object
             *
             */
            ~State(void) = default;

            /**
             * @brief Ask the state if she is running
             *
             * @return bool True if the state is running
             */
            bool isRunning(void) const;

            /**
             * @brief Get the tag object.
             *
             * @return uint32_t The state tag
             */
            uint32_t getTag(void) const;

            /**
             * @brief Call the init function of the state
             *
             * @param app The app program
             */
            void init(App &app);

            /**
             * @brief Call the update function of the state
             *
             * @param app The app program
             */
            void update(App &app);

            /**
             * @brief Call the exit function of the state
             *
             * @param app The app program
             */
            void exit(App &app);
        };

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

    } // namespace core
} // namespace vazel
