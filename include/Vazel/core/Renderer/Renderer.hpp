/**
 * include/Vazel/core/Renderer/Renderer.hpp
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
#include <SFML/Graphics.hpp>
#include <list>
#include <variant>

namespace vazel
{
    namespace core
    {

        enum RenderType
        {
            CIRCLE_SHAPE,
            CONVEX_SHAPE,
            SPRITE,
            TEXT,
            VERTEX_ARRAY,
            VERTEX_BUFFER
        };

        using DrawableData =
            std::variant<sf::CircleShape, sf::ConvexShape, sf::Sprite,
                         sf::Text, sf::VertexArray, sf::VertexBuffer>;

        class Drawable
        {
          private:
            DrawableData _data;
            RenderType _type;
            int _layer       = 0;
            bool _shouldDraw = true;

          public:
            Drawable(sf::CircleShape &circleshape);

            sf::CircleShape &getCircleShape(void);
            const sf::CircleShape &getCircleShape(void) const;

            Drawable(sf::ConvexShape &convexshape);

            sf::ConvexShape &getConvexShape(void);
            const sf::ConvexShape &getConvexShape(void) const;

            Drawable(sf::Sprite &sprite);

            sf::Sprite &getSprite(void);
            const sf::Sprite &getSprite(void) const;

            Drawable(sf::Text &text);

            sf::Text &getText(void);
            const sf::Text &getText(void) const;

            Drawable(sf::VertexArray &vao);

            sf::VertexArray &getVertexArray(void);
            const sf::VertexArray &getVertexArray(void) const;

            Drawable(sf::VertexBuffer &vbo);

            sf::VertexBuffer &getVertexBuffer(void);
            const sf::VertexBuffer &getVertexBuffer(void) const;

            void setLayer(int layer);

            void setShouldDraw(bool set);

            const bool &shouldDraw(void) const;

            const int &getLayer(void) const;

            const RenderType &getRenderType(void) const;
        };

        class Renderer
        {
          private:
            std::list<Drawable> _drawables;

          public:
            Renderer(void)  = default;
            ~Renderer(void) = default;

            void sort(void);

            Drawable *addDrawable(const Drawable &drawable);

            template <typename T>
            void draw(sf::RenderWindow &window, const T &drawable) const
            {
                window.draw(drawable);
            }

            void draw(sf::RenderWindow &window,
                      const Drawable &drawable) const;

            void drawList(sf::RenderWindow &window);

            template <typename T>
            static Drawable *addDrawable(Renderer &renderer, const T &data)
            {
                return renderer.addDrawable(Drawable(data));
            }

            template <typename T>
            static Drawable *addDrawable(Renderer &renderer, const T data)
            {
                return renderer.addDrawable(Drawable(data));
            }
        };

        class Framebuffer
        {
          private:
            sf::Texture _texture;
            sf::Sprite _sprite;
            sf::Uint8 *_pixels = nullptr;
            unsigned int _width;
            unsigned int _height;

            void writeColorToPixelBuffer(const unsigned int rawIndex,
                                         const sf::Color color);

          public:
            Framebuffer(const unsigned int x, const unsigned int y);

            Framebuffer(const sf::RenderWindow &window);

            ~Framebuffer(void);

            const unsigned int rawFrambufferSize(void) const;

            const unsigned int framebufferSize(void) const;

            const unsigned int &getWidth(void) const;

            const unsigned int &getHeight(void) const;

            unsigned int getIndex(const unsigned int x,
                                  const unsigned int y) const;

            void put(const unsigned int x, const unsigned int y,
                     const sf::Color color);

            void clear(const sf::Color color);

            void updateTexture(void);

            void draw(sf::RenderWindow &window, bool update);
        };

    } // namespace core
} // namespace vazel
