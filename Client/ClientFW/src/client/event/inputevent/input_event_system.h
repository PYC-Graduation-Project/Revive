#pragma once
#include "client/event/event_system.h"

namespace client_fw
{
    struct Window;
    class InputManager;
    class InputEventManager;

    class InputEventSystem final : public IEventSystem
    {
    public:
        InputEventSystem(const WPtr<Window>& window);
        virtual ~InputEventSystem();

        InputEventSystem(const InputEventSystem&) = delete;
        InputEventSystem& operator=(const InputEventSystem&) = delete;

        virtual void ExecuteEvent() override;

    public:
        void ChangeInputState(UINT message, WPARAM wParam, LPARAM lParam);

    private:
        UPtr<InputManager> m_input_manager;
        UPtr<InputEventManager> m_input_event_manager;

    public:
        const UPtr<InputManager>& GetInputManager() const { return m_input_manager; }
        const UPtr<InputEventManager>& GetInputEventManager() const { return m_input_event_manager; }
    };

}

