// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include "luGUIModule.hpp"

export module luGUI.UserInterface.Items.Button;

export import luGUI.UserInterface.Items.Item;

namespace luGUI
{
    export class LUGUIMODULE_API Button : public Item
    {
        strzilla::string      m_Label {};
        std::function<void()> m_OnClicked {};

    public:
        Button() = default;
        explicit Button(strzilla::string_view const &Label);
        explicit Button(strzilla::string_view const &Label, std::function<void()> &&OnClicked);

        [[nodiscard]] inline strzilla::string_view GetLabel() const
        {
            return m_Label;
        }

        inline void SetKey(strzilla::string_view const &Label)
        {
            m_Label = Label;
        }

        inline void SetOnClicked(std::function<void()> &&OnClicked)
        {
            m_OnClicked = OnClicked;
        }

        void Draw() override;
    };
} // namespace luGUI
