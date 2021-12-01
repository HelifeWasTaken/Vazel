#include "Vazel/core/State.hpp"

namespace vazel
{
    namespace core
    {

        State::State(std::function<void(App &)> on_init,
                     std::function<bool(App &)> on_update,
                     std::function<void(App &)> on_exit, uint32_t tag)
            : _on_init(on_init)
            , _on_update(on_update)
            , _on_exit(on_exit)
            , _tag(tag)
        {
        }

        const bool State::isRunning(void)
        {
            return _is_running;
        }

        const uint32_t &State::getTag(void)
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

        const std::vector<std::shared_ptr<State>>::iterator getStateFromTag(
            std::vector<std::shared_ptr<State>> &states, uint32_t stateTag)

        {
            return std::find_if(states.begin(), states.end(),
                                [&](std::shared_ptr<State> &state) {
                                    return state->getTag() == stateTag;
                                });
        }

    } // namespace core
} // namespace vazel