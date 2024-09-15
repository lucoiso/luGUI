// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

module luGUI.UserInterface.Items.Text;

import luGUI.UserInterface.Singletons.FontManager;

using namespace luGUI;

Text::Text(strzilla::string_view const &Key)
    : m_Key(Key)
{
}

Text::Text(strzilla::string_view const &Key, strzilla::string_view const &Text)
    : m_Key(Key)
  , m_Text(Text)
{
}

void Text::Draw()
{
    ImFont *const Font = FontManager::Get().GetFont(m_Key);
    if (Font)
    {
        ImGui::PushFont(Font);
    }

    ImGui::Text(std::data(m_Text));

    if (Font)
    {
        ImGui::PopFont();
    }
}
