// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include "luGUIModule.hpp"

export module luGUI.UserInterface.Items.Text;

export import luGUI.UserInterface.Items.Item;

namespace luGUI
{
    export class LUGUIMODULE_API Text : public Item
    {
        strzilla::string m_Key {};
        strzilla::string m_Text {};

    public:
        Text() = default;
        explicit Text(float Width);
        explicit Text(strzilla::string_view const &Key, float Width = 0.F);
        explicit Text(strzilla::string_view const &Key, strzilla::string_view const &Text, float Width = 0.F);

        [[nodiscard]] inline strzilla::string_view GetKey() const
        {
            return m_Key;
        }

        inline void SetKey(strzilla::string_view const &Key)
        {
            m_Key = Key;
        }

        [[nodiscard]] inline strzilla::string_view GetText() const
        {
            return m_Text;
        }

        inline void SetText(strzilla::string_view const &Text)
        {
            m_Text = Text;
        }

    protected:
        void Render() override;
    };
} // namespace luGUI
