// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include <imgui.h>

module luGUI.UserInterface.Items.Text;

import luGUI.UserInterface.Singletons.FontManager;

using namespace luGUI;

Text::Text(float const Width)
    : Item(Width)
{
}

Text::Text(strzilla::string_view const &Key, float const Width)
    : Item(Width)
  , m_Key(Key)
{
}

Text::Text(strzilla::string_view const &Key, strzilla::string_view const &Text, float const Width)
    : Item(Width)
  , m_Key(Key)
  , m_Text(Text)
{
}

void Text::Render()
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
