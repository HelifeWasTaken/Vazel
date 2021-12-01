#pragma once

#include "Vazel/core/App.hpp"

#include <cstdint>
#include <functional>
#include <memory>

namespace vazel
{
    namespace core
    {

        class App;

        class State
        {
          private:
            std::function<void(App &)> _on_init;
            std::function<bool(App &)> _on_update;
            std::function<void(App &)> _on_exit;
            uint32_t _tag;
            bool _is_running = false;

          public:
            State(std::function<void(App &)> on_init,
                  std::function<bool(App &)> on_update,
                  std::function<void(App &)> on_exit, uint32_t tag);

            ~State(void) = default;

            const bool isRunning(void);

            const uint32_t &getTag(void);

            void init(App &app);

            void update(App &app);

            void exit(App &app);
        };

        const std::vector<std::shared_ptr<State>>::iterator getStateFromTag(
            std::vector<std::shared_ptr<State>> &states, uint32_t stateTag);

    } // namespace core
} // namespace vazel
