// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include <imgui.h>

module luGUI.UserInterface.Items.Text;

import luGUI.UserInterface.Singletons.FontManager;

using namespace luGUI;

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
