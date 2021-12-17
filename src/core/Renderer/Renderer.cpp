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

        Drawable::Drawable(sf::CircleShape& circleshape)
            : _data(circleshape)
            , _type(CIRCLE_SHAPE)
        {
        }

        sf::CircleShape& Drawable::getCircleShape(void)
        {
            return std::get<sf::CircleShape>(_data);
        }

        static void s_drawCircleShape(sf::RenderWindow& window,
                                      Drawable& drawable)
        {
            window.draw(drawable.getCircleShape());
        }

        Drawable::Drawable(sf::ConvexShape& convexshape)
            : _data(convexshape)
            , _type(CONVEX_SHAPE)
        {
        }

        sf::ConvexShape& Drawable::getConvexShape(void)
        {
            return std::get<sf::ConvexShape>(_data);
        }

        static void s_drawConvexShape(sf::RenderWindow& window,
                                      Drawable& drawable)
        {
            window.draw(drawable.getConvexShape());
        }

        Drawable::Drawable(sf::Sprite& sprite)
            : _data(sprite)
            , _type(SPRITE)
        {
        }

        sf::Sprite& Drawable::getSprite(void)
        {
            return std::get<sf::Sprite>(_data);
        }

        static void s_drawSprite(sf::RenderWindow& window, Drawable& drawable)
        {
            window.draw(drawable.getSprite());
        }

        Drawable::Drawable(sf::Text& text)
            : _data(text)
            , _type(TEXT)
        {
        }

        sf::Text& Drawable::getText(void)
        {
            return std::get<sf::Text>(_data);
        }

        static void s_drawText(sf::RenderWindow& window, Drawable& drawable)
        {
            window.draw(drawable.getText());
        }

        Drawable::Drawable(sf::VertexArray& vao)
            : _data(vao)
            , _type(VERTEX_ARRAY)
        {
        }

        sf::VertexArray& Drawable::getVertexArray(void)
        {
            return std::get<sf::VertexArray>(_data);
        }

        static void s_drawVertexArray(sf::RenderWindow& window,
                                      Drawable& drawable)
        {
            window.draw(drawable.getVertexArray());
        }

        Drawable::Drawable(sf::VertexBuffer& vbo)
            : _data(vbo)
            , _type(VERTEX_BUFFER)
        {
        }

        sf::VertexBuffer& Drawable::getVertexBuffer(void)
        {
            return std::get<sf::VertexBuffer>(_data);
        }

        static void s_drawVertexBuffer(sf::RenderWindow& window,
                                       Drawable& drawable)
        {
            window.draw(drawable.getVertexBuffer());
        }

        void Drawable::setLayer(int layer)
        {
            _layer = layer;
        }

        void Drawable::setShouldDraw(bool set)
        {
            _shouldDraw = set;
        }

        const bool& Drawable::shouldDraw(void) const
        {
            return _shouldDraw;
        }

        const int& Drawable::getLayer(void) const
        {
            return _layer;
        }

        const RenderType& Drawable::getRenderType(void) const
        {
            return _type;
        }

        void Renderer::sort(void)
        {
            _drawables.sort([](const Drawable& a, const Drawable& b) {
                return a.getLayer() < b.getLayer();
            });
        }

        Drawable* Renderer::addDrawable(Drawable& drawable)
        {
            _drawables.push_front(drawable);
            return &_drawables.front();
        }

        void Renderer::draw(sf::RenderWindow& window, Drawable& drawable)
        {
            static void (*fdraw[])(sf::RenderWindow&, Drawable&) = {
                s_drawCircleShape, s_drawConvexShape, s_drawSprite,
                s_drawText,        s_drawVertexArray, s_drawVertexBuffer
            };

            fdraw[drawable.getRenderType()](window, drawable);
        }

        void Renderer::drawList(sf::RenderWindow& window)
        {
            sort();

            for (auto it : _drawables) {
                if (it.shouldDraw() == true) {
                    draw(window, it);
                }
            }
        }

        void Framebuffer::writeColorToPixelBuffer(const unsigned int rawIndex,
                                                  const sf::Color color)
        {
            _pixels[rawIndex]     = color.r;
            _pixels[rawIndex + 1] = color.g;
            _pixels[rawIndex + 2] = color.b;
            _pixels[rawIndex + 3] = color.a;
        }

        Framebuffer::Framebuffer(const unsigned int x, const unsigned int y)
            : _width(x)
            , _height(y)
        {
            _texture.create(_width, _height);
            _pixels = new sf::Uint8[rawFrambufferSize()];
        }

        Framebuffer::Framebuffer(const sf::RenderWindow& window)
        {
            sf::Vector2u size = window.getSize();

            *this = Framebuffer(size.x, size.y);
        }

        Framebuffer::~Framebuffer(void)
        {
            delete _pixels;
        }

        const unsigned int Framebuffer::rawFrambufferSize(void) const
        {
            return _width * _height * 4;
        }

        const unsigned int Framebuffer::framebufferSize(void) const
        {
            return _width * _height;
        }

        const unsigned int& Framebuffer::getWidth(void) const
        {
            return _width;
        }

        const unsigned int& Framebuffer::getHeight(void) const
        {
            return _height;
        }

        unsigned int Framebuffer::getIndex(const unsigned int x,
                                           const unsigned int y) const
        {
            return (x + y * _width) * 4;
        }

        void Framebuffer::put(const unsigned int x, const unsigned int y,
                              const sf::Color color)
        {
            if (x > _width || y > _height) {
                throw std::runtime_error("Invalid position in framebuffer");
            }
            writeColorToPixelBuffer(getIndex(x, y), color);
        }

        void Framebuffer::clear(const sf::Color color)
        {
            const unsigned int framebuffer_size = _width * _height * 4;

            for (unsigned int i = 0; i < framebuffer_size; i += 4) {
                writeColorToPixelBuffer(i, color);
            }
        }

        void Framebuffer::updateTexture(void)
        {
            _texture.update(_pixels);
            _sprite.setTexture(_texture);
        }

        void Framebuffer::draw(sf::RenderWindow& window, bool update = true)
        {
            if (update)
                updateTexture();
            window.draw(_sprite);
        }

    } // namespace core
} // namespace vazel
