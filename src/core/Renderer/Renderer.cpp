/**
 * src/Vazel/core/Renderer/Renderer.cpp
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
#include "Vazel/core/Renderer/Renderer.hpp"

namespace vazel
{
    namespace core
    {

        int64_t Drawable::getLayer(void) const { return _layer; }
        bool Drawable::shouldDraw(void) const { return _shouldDraw; }
        void Drawable::setLayer(int64_t layer) { _layer = layer; }
        void Drawable::setShouldDraw(bool shouldDraw) { _shouldDraw = shouldDraw; }

        void Renderer::sort(void)
        {
            std::sort(_drawables.begin(), _drawables.end(),
                [](const auto& a, const auto& b) {
                    return a->getLayer() < b->getLayer();
                }
            );
        }

        void Renderer::draw(sf::RenderWindow &window, bool sort)
        {
            if (sort)
                this->sort();
            for (const auto& it : _drawables) {
                if (it->shouldDraw() == true) {
                    window.draw(*it);
                }
            }
        }

        void Renderer::removeDrawable(DrawableElement& drawable)
        {
            std::remove_if(_drawables.begin(), _drawables.end(),
                [&drawable](const std::shared_ptr<IDrawable>& it) {
                    return it == drawable;
                }
            );
        }

    } // namespace core
} // namespace vazel
