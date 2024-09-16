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
        template <typename... Args>
        explicit constexpr Text(const char *const Key, const char *const &Text, Args &&... Arguments)
            : Item(std::forward<Args>(Arguments)...)
          , m_Key(Key)
          , m_Text(Text)
        {
        }

        [[nodiscard]] inline strzilla::string_view GetKey() const
        {
            return m_Key;
        }

        inline void SetKey(const char *const Key)
        {
            m_Key = Key;
        }

        [[nodiscard]] inline strzilla::string_view GetText() const
        {
            return m_Text;
        }

        inline void SetText(const char *const &Text)
        {
            m_Text = Text;
        }

    protected:
        void Render() override;
    };
} // namespace luGUI
