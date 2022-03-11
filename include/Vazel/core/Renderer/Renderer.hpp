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
#include <vector>
#include <memory>

namespace vazel
{
    namespace core
    {

        /**
         * @brief Interface that implements the rendering of the game
         * Permits to be added to the renderer
         * The drawable must provide an implementation of draw
         * in accordance to the sfml drawable interface
         * 
         */
        class IDrawable : public sf::Drawable
        {
        public:
            /**
             * @brief Get the Layer object
             * 
             * @return int64_t The layer
             */
            virtual int64_t getLayer(void) const = 0;

            /**
             * @brief Permit to know if the object is visible
             * 
             * @return true The object is visible
             * @return false The object is not visible
             */
            virtual bool shouldDraw(void) const = 0;

            /**
             * @brief Set the Layer object
             * 
             * @param layer The Layer
             */
            virtual void setLayer(int64_t layer) = 0;

            /**
             * @brief Set the Should Draw object
             * 
             * @param shouldDraw 
             */
            virtual void setShouldDraw(bool shouldDraw) = 0;

            /**
             * @brief Destroy the IDrawable interface
             * 
             */
            virtual ~IDrawable() = default;
        };

        /**
         * @brief Abstract class that implements the rendering of the game
         * Permits to be added to the renderer
         * The drawable must provide an implementation of draw
         * in accordance to the sfml drawable interface
         */
        class Drawable : public IDrawable
        {
        private:
            int64_t _layer;
            bool _shouldDraw;

        public:
            /**
             * @brief Construct a new Drawable object
             * 
             * @param layer Where the object is drawn
             * @param shouldDraw If the object is visible by default
             */
            Drawable(int64_t layer=0, bool shouldDraw=true);

            /**
             * @brief Get the Layer object
             * 
             * @return int64_t The layer
             */
            int64_t getLayer(void) const override final;

            /**
             * @brief If the object is visible
             * 
             * @return true The object is visible
             * @return false The object is not visible
             */
            bool shouldDraw(void) const override final;

            /**
             * @brief Set the Layer object
             * 
             * @param layer The Layer
             */
            void setLayer(int64_t layer) override final;
            /**
             * @brief Set the Should Draw object
             * 
             * @param shouldDraw If the object is visible
             */
            void setShouldDraw(bool shouldDraw) override final;

            /**
             * @brief Destroy the Drawable Abstract
             * 
             */
            ~Drawable() override = default;
        };

        /**
         * @brief Implements the rendering of the game for Base SFML builtins
         * 
         * @tparam T The SFML builtin
         */
        template<typename T>
        class ABaseSFML : public Drawable
        {
        private:
            T _impl;

        public:
            /**
             * @brief Get the SFML object
             * 
             * @return T& The SFML object
             */
            T &get(void) { return _impl; }

            /**
             * @brief Get the SFML object
             * 
             * @return const T& The SFML object
             */
            const T &get(void) const { return _impl; }

            /**
             * @brief Construct a new ABaseSFML object
             * 
             * @param layer The layer
             * @param shouldDraw If the object is visible by default
             */
            ABaseSFML(int64_t layer=0, bool shouldDraw=true) :
                Drawable(layer, shouldDraw)
            {}

            /**
             * @brief Draw the SFML object
             * 
             * @param target The target
             * @param states The states
             */
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override
            { target.draw(_impl); }

            /**
             * @brief Destroy the ABaseSFML class
             * 
             */
            ~ABaseSFML() override = default;
        };

        /**
         * @brief Abstraction of the SFML builtin
         */
        using InternalSprite = sf::Sprite;

        /**
         * @brief Abstraction of the SFML builtin
         */
        using InternalText = sf::Text;

        /**
         * @brief Abstraction of the SFML builtin
         */
        using InternalVertex = sf::Vertex;

        /**
         * @brief Abstraction of the SFML builtin
         */
        using InternalVertexBuffer = sf::VertexBuffer;

        /**
         * @brief Abstraction of the SFML builtin
         */
        using InternalVertexArray = sf::VertexArray;

        /**
         * @brief Drawable implementation of the SFML builtin
         */
        using Sprite = ABaseSFML<InternalSprite>;

        /**
         * @brief Drawable implementation of the SFML builtin
         */
        using Text = ABaseSFML<InternalText>;

        /**
         * @brief Drawable implementation of the SFML builtin
         */
        using VertexBuffer = ABaseSFML<sf::VertexBuffer>;

        /**
         * @brief Drawable implementation of the SFML builtin
         */
        using VertexArray = ABaseSFML<sf::VertexArray>;

        /**
         * @brief List of the drawable objects
         * 
         */
        class Renderer
        {
          public:
            using DrawableElement = std::shared_ptr<IDrawable>;
            using DrawableList = std::vector<DrawableElement>;
          private:
            DrawableList _drawables;

          public:
            /**
             * @brief Construct a new Renderer object
             * 
             */
            Renderer(void)  = default;
            /**
             * @brief Destroy the Renderer object
             * 
             */
            ~Renderer(void) = default;

            /**
             * @brief Sort the drawables by layer
             * 
             */
            void sort(void);

            /**
             * @brief Add a drawable object
             * 
             * @param window The window
             * @param sort If the drawables must be sorted
             */
            void draw(sf::RenderWindow &window, bool sort=true);

            /**
             * @brief Add a drawable object
             * 
             * @tparam T The type of the drawable
             * @return Drawable* A pointer to the drawable
             */
            template<typename T>
            DrawableElement& addDrawable(void)
            {
                _drawables.push_back(std::make_shared<T>());
                return std::shared_ptr<T>(_drawables.back().get());
            }

            /**
             * @brief remove a drawable object 
             * 
             * @param drawable The drawable to remove
             */
            void removeDrawable(DrawableElement& element);

            /**
             * @brief Get the Drawables object
             * 
             * @param window The window
             */
            void drawList(sf::RenderWindow &window);
        };

    } // namespace core
} // namespace vazel
