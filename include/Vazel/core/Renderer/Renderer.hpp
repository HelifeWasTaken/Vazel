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
            Drawable(sf::CircleShape& circleshape);

            sf::CircleShape& getCircleShape(void);

            Drawable(sf::ConvexShape& convexshape);

            sf::ConvexShape& getConvexShape(void);

            Drawable(sf::Sprite& sprite);

            sf::Sprite& getSprite(void);

            Drawable(sf::Text& text);

            sf::Text& getText(void);

            Drawable(sf::VertexArray& vao);

            sf::VertexArray& getVertexArray(void);

            Drawable(sf::VertexBuffer& vbo);

            sf::VertexBuffer& getVertexBuffer(void);

            void setLayer(int layer);

            void setShouldDraw(bool set);

            const bool& shouldDraw(void) const;

            const int& getLayer(void) const;

            const RenderType& getRenderType(void) const;
        };

        class Renderer
        {
          private:
            std::list<Drawable> _drawables;

          public:
            Renderer(void)  = default;
            ~Renderer(void) = default;

            void sort(void);

            Drawable* addDrawable(Drawable& drawable);

            void draw(sf::RenderWindow& window, Drawable& drawable);

            void drawList(sf::RenderWindow& window);
        };

    } // namespace core
} // namespace vazel
