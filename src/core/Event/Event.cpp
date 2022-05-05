/**
 * src/core/Event/Event.cpp
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

#include "Vazel/core/Event/Event.hpp"

namespace vazel
{
    namespace core
    {
        std::mutex Keyboard::_keyboardMut;
        std::queue<Keyboard::Input> Keyboard::_events;

        bool Keyboard::poll(Keyboard::Input& type)
        {
            _keyboardMut.lock();
            if (_events.empty()) {
                _keyboardMut.unlock();
                return false;
            }
            type = _events.front();
            _events.pop();
            _keyboardMut.unlock();
            return true;
        }

        void Keyboard::pushEvent(const Keyboard::Input& type)
        {
            _keyboardMut.lock();
            _events.push(type);
            _keyboardMut.unlock();
        }
    } // namespace core
} // namespace vazel
