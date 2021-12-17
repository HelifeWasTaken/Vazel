/**
 * src/core/State.cpp
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
#include "Vazel/core/State/State.hpp"

namespace vazel
{
    namespace core
    {

        StateException::StateException(const std::string &e)
            : _e(e)
        {
        }

        const char *StateException::what() const throw()
        {
            return _e.c_str();
        }

        State::State(std::function<void(App &)> on_init,
                     std::function<void(App &)> on_update,
                     std::function<void(App &)> on_exit, uint32_t tag)
            : _on_init(on_init)
            , _on_update(on_update)
            , _on_exit(on_exit)
            , _tag(tag)
            , _is_running(false)
        {
        }

        bool State::isRunning(void) const
        {
            return _is_running;
        }

        uint32_t State::getTag(void) const
        {
            return _tag;
        }

        void State::init(App &app)
        {
            _is_running = true;
            _on_init(app);
        }

        void State::update(App &app)
        {
            _on_update(app);
        }

        void State::exit(App &app)
        {
            _is_running = false;
            _on_exit(app);
        }

        void basicOnUpdate(App &app)
        {
            sf::Event evt;

            while (app.window.isOpen()) {
                app.window.clear();
                while (app.window.pollEvent(evt)) {
                    if (evt.type == sf::Event::Closed)
                        app.stop();
                    else
                        app.world.updateOnEventSystem(evt);
                }
                app.world.updateSystem();
                app.renderer.drawList(app.window);
            }
        }

    } // namespace core
} // namespace vazel
