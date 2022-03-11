#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace vazel
{
    namespace core
    {
        template <typename R>
        class ResourceHolder
        {
          private:
            void _insertResource(const std::string &id,
                                 std::unique_ptr<R> &resource)
            {
                auto r = _resource_map.insert(
                    std::make_pair(id, std::move(resource)));
                assert(r.second);
            }

          private:
            std::unordered_map<std::string, std::unique_ptr<R>> _resource_map;

          public:
            void load(const std::string &filename)
            {
                std::unique_ptr<R> nr(new R());

                if (nr->loadFromFile(filename) == false) {
                    throw std::runtime_error("Coud not load filename");
                }
                insertResource(filename, nr);
            }

            template <typename P>
            void load(const std::string &filename, const P &param)
            {
                std::unique_ptr<R> nr(new R());

                if (nr->loadFromFile(filename, param) == false) {
                    throw std::runtime_error("Coud not load filename");
                }
                insertResource(filename, nr);
            }

            const R *getp(const std::string &id) const
            {
                auto f = _resource_map.find(id);

                assert(f != _resource_map.end());
                return f->second;
            }

            R *getp(const std::string &id)
            {
                auto f = _resource_map.find(id);

                assert(f != _resource_map.end());
                return f->second;
            }

            const R &get(const std::string &id) const
            {
                return *getp(id);
            }

            R &get(const std::string &id)
            {
                return *getp(id);
            }
        };

        struct ResourceManager
        {
            ResourceHolder<sf::Music> musicManager;

            ResourceHolder<sf::Sound> soundManager;
            ResourceHolder<sf::SoundBuffer> soundBufferManager;

            ResourceHolder<sf::Font> fontManager;

            ResourceHolder<sf::Image> imageManager;
            ResourceHolder<sf::Texture> textureManager;
        };

    }
}
