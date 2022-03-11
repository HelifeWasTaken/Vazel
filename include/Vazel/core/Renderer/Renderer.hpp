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

        class IDrawable : public sf::Drawable
        {
        public:
            virtual int64_t getLayer(void) const = 0;
            virtual bool shouldDraw(void) const = 0;

            virtual void setLayer(int64_t layer) = 0;
            virtual void setShouldDraw(bool shouldDraw) = 0;

            virtual ~IDrawable() = default;
        };

        class Drawable : public IDrawable
        {
        private:
            int64_t _layer;
            bool _shouldDraw;

        public:
            Drawable(int64_t layer=0, bool shouldDraw=true);

            int64_t getLayer(void) const override final;
            bool shouldDraw(void) const override final;

            void setLayer(int64_t layer) override final;
            void setShouldDraw(bool shouldDraw) override final;

            ~Drawable() override = default;
        };

        template<typename T>
        class ABaseSFML : public Drawable
        {
        private:
            T _impl;

        public:
            T &get(void) { return _impl; }
            const T &get(void) const { return _impl; }

            ABaseSFML(int64_t layer=0, bool shouldDraw=true) :
                Drawable(layer, shouldDraw)
            {}

            void draw(sf::RenderTarget& target, sf::RenderStates states) const override
            { target.draw(_impl); }

            ~ABaseSFML() override = default;
        };

        using InternalSprite = sf::Sprite;
        using InternalText = sf::Text;
        using InternalVertex = sf::Vertex;
        using InternalVertexBuffer = sf::VertexBuffer;
        using InternalVertexArray = sf::VertexArray;

        using Sprite = ABaseSFML<InternalSprite>;
        using Text = ABaseSFML<InternalText>;
        using VertexBuffer = ABaseSFML<sf::VertexBuffer>;
        using VertexArray = ABaseSFML<sf::VertexArray>;

        class Renderer
        {
          private:
            std::vector<std::unique_ptr<IDrawable>> _drawables;

          public:
            Renderer(void)  = default;
            ~Renderer(void) = default;

            void sort(void);
            void draw(sf::RenderWindow &window, bool sort=true);

            Drawable *addDrawable(const Drawable &drawable);
            void removeDrawable(const Drawable *drawable);

            void drawList(sf::RenderWindow &window);
        };

    } // namespace core
} // namespace vazel
