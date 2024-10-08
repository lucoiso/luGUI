// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include "luGUIModule.hpp"

#include <imgui.h>

export module luGUI.UserInterface.Items.Input;

export import luGUI.UserInterface.Items.Item;

namespace luGUI
{
    export LUGUIMODULE_API std::int32_t InputTextCallback(ImGuiInputTextCallbackData *);

    export struct LUGUIMODULE_API NullInputSettings
    {
    };

    constexpr NullInputSettings g_NullInputSettings {};

    export struct LUGUIMODULE_API TextInputSettings : NullInputSettings
    {
        bool   Multiline { false };
        ImVec2 Size { 0.F, 0.F };
    };

    export template <typename T>
    struct LUGUIMODULE_API NumberInputSettings : NullInputSettings
    {
        bool        Slider { false };
        T           Step { std::is_integral_v<T> ? 1 : 0 };
        T           MaxValue { 100 };
        T           MinValue { 0 };
        const char *Format { std::is_integral_v<T> ? "%d" : "%.3F" };
    };

    export template <typename ValueType, typename ConfigType = NullInputSettings>
    class LUGUIMODULE_API Input : public Item
    {
        ValueType                      m_CurrentInput {};
        ConfigType                     m_Config { g_NullInputSettings };
        strzilla::string               m_Label {};
        strzilla::string               m_ID {};
        std::function<void(ValueType)> m_OnChanged {};

    public:
        template <typename... Args>
        explicit Input(const char *const Label, std::function<void(ValueType)> &&OnChanged, Args &&... Arguments)
            : Item(std::forward<Args>(Arguments)...)
          , m_Label(Label)
          , m_ID(strzilla::string { "##" } + m_Label)
          , m_OnChanged(OnChanged)
        {
        }

        template <typename... Args>
        explicit Input(const char *const Label, std::function<void(ValueType)> &&OnChanged, ConfigType const &Config, Args &&... Arguments)
            : Item(std::forward<Args>(Arguments)...)
          , m_Config(Config)
          , m_Label(Label)
          , m_ID(strzilla::string { "##" } + m_Label)
          , m_OnChanged(OnChanged)
        {
        }

        [[nodiscard]] inline strzilla::string_view GetLabel() const
        {
            return m_Label;
        }

        inline void SetKey(const char *const Label)
        {
            m_Label = Label;
        }

        [[nodiscard]] inline ConfigType const &GetConfig() const
        {
            return m_Config;
        }

        inline void SetConfig(ConfigType &&Config)
        {
            m_Config = Config;
        }

        inline void SetOnChanged(std::function<void(ValueType)> &&OnChanged)
        {
            m_OnChanged = OnChanged;
        }

    protected:
        void Render() override
        {
            using DecayedInputType  = std::decay_t<ValueType>;
            using DecayedConfigType = std::decay_t<ConfigType>;

            bool Commited = false;

            ImGui::Text(std::data(m_Label));
            ImGui::SameLine();

            if constexpr (std::is_same_v<DecayedInputType, strzilla::string>)
            {
                if constexpr (std::is_same_v<DecayedConfigType, TextInputSettings>)
                {
                    if (m_Config.Multiline)
                    {
                        Commited = ImGui::InputTextMultiline(std::data(m_ID),
                                                             std::data(m_CurrentInput),
                                                             std::size(m_CurrentInput),
                                                             m_Config.Size,
                                                             ImGuiInputTextFlags_CallbackResize,
                                                             InputTextCallback,
                                                             static_cast<void *>(&m_CurrentInput));
                    }
                    else
                    {
                        Commited = ImGui::InputText(std::data(m_ID),
                                                    std::data(m_CurrentInput),
                                                    std::size(m_CurrentInput),
                                                    ImGuiInputTextFlags_CallbackResize,
                                                    InputTextCallback,
                                                    static_cast<void *>(&m_CurrentInput));
                    }
                }
                else
                {
                    Commited = ImGui::InputText(std::data(m_ID),
                                                std::data(m_CurrentInput),
                                                std::size(m_CurrentInput),
                                                ImGuiInputTextFlags_CallbackResize,
                                                InputTextCallback,
                                                static_cast<void *>(m_CurrentInput.c_str()));
                }
            }
            else if constexpr (std::is_arithmetic_v<DecayedInputType>)
            {
                if constexpr (std::is_floating_point_v<DecayedInputType>)
                {
                    if constexpr (std::is_same_v<DecayedConfigType, NumberInputSettings<ValueType>>)
                    {
                        if (m_Config.Slider)
                        {
                            Commited = ImGui::SliderFloat(std::data(m_ID), &m_CurrentInput, m_Config.MinValue, m_Config.MaxValue, m_Config.Format);
                        }
                        else
                        {
                            Commited = ImGui::InputFloat(std::data(m_ID), &m_CurrentInput, m_Config.Step, m_Config.Step, m_Config.Format);
                        }
                    }
                    else
                    {
                        Commited = ImGui::InputFloat(std::data(m_ID), &m_CurrentInput);
                    }
                }
                else
                {
                    if constexpr (std::is_same_v<DecayedConfigType, NumberInputSettings<ValueType>>)
                    {
                        if (m_Config.Slider)
                        {
                            Commited = ImGui::SliderInt(std::data(m_ID), &m_CurrentInput, m_Config.MinValue, m_Config.MaxValue, m_Config.Format);
                        }
                        else
                        {
                            Commited = ImGui::InputInt(std::data(m_ID), &m_CurrentInput, m_Config.Step, m_Config.Step);
                        }
                    }
                    else
                    {
                        Commited = ImGui::InputInt(std::data(m_ID), &m_CurrentInput);
                    }
                }
            }

            if (Commited && m_OnChanged)
            {
                m_OnChanged(m_CurrentInput);
            }
        }
    };

    export using TextInput  = Input<strzilla::string, TextInputSettings>;
    export using FloatInput = Input<float, NumberInputSettings<float>>;
    export using IntInput   = Input<std::int32_t, NumberInputSettings<std::int32_t>>;
} // namespace luGUI
