/**
 * src/core/App.cpp
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
#include "Vazel/core/App/App.hpp"

namespace vazel
{
    namespace core
    {

        AppException::AppException(const std::string &e)
            : _e(e)
        {
        }

        const char *AppException::what() const throw()
        {
            return _e.c_str();
        }

        void App::stop(void)
        {
            _pending_state = nullptr;
            if (_current_state != nullptr) {
                _current_state->exit(*this);
            }
        }

        void App::setState(uint32_t stateTag)
        {
            const auto it = getStateFromTag(_states, stateTag);

            if (it == _states.end()) {
                char buf[BUFSIZ] = { 0 };
                snprintf(buf, sizeof(buf) - 1,
                         "world::setCurrentState: Could not find State with "
                         "tag: \"%u\"",
                         stateTag);
                throw AppException(buf);
            }
            _pending_state = it->get();
            if (_current_state != nullptr) {
                _current_state->exit(*this);
            }
        }

        void App::registerState(State &state)
        {
            _states.push_back(std::make_shared<State>(state));
            setState(state.getTag());
        }

        App::App(State &baseState)
        {
            registerState(baseState);
        }

        void App::run(void)
        {
            if (_pending_state == nullptr)
                throw AppException("There is no pending scene right now");
            while (_pending_state != nullptr) {
                _current_state = _pending_state;
                _current_state->init(*this);
                _pending_state = nullptr;
                while (_current_state->isRunning()) {
                    _current_state->update(*this);
                }
                world.clearWorld();
            }
            _current_state = nullptr;
        }

        App *g_app = nullptr;

    } // namespace core
} // namespace vazel