// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include "luGUIModule.hpp"

export module luGUI.UserInterface.Window;

import luGUI.Integrations.GLFWHandler;

import luGUI.UserInterface.Control;
import luGUI.UserInterface.Window.Flags;

namespace luGUI
{
    export class LUGUIMODULE_API Window : public Control
    {
        bool                m_PendingClose{false};
        InitializationFlags m_Flags{};
        GLFWHandler         m_GLFWHandler{};
        std::uint16_t       m_Width{};
        std::uint16_t       m_Height{};
        strzilla::string    m_Title{};

    public:
        Window();

        Window(Window const &) = delete;
        Window &operator=(Window const &) = delete;
        ~Window() override = default;

        bool Initialize(std::uint16_t, std::uint16_t, strzilla::string_view, InitializationFlags Flags = InitializationFlags::NONE);
        void RequestClose();
        void Shutdown();
        [[nodiscard]] bool IsOpen() const;

        virtual void PollEvents();

    protected:
        void SetCallbacks();
        void Draw();
    };
} // namespace luGUI
