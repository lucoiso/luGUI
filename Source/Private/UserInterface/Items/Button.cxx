// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include <imgui.h>

module luGUI.UserInterface.Items.Button;

using namespace luGUI;

void Button::Render()
{
    if (ImGui::Button(std::data(m_Label)) && m_OnClicked)
    {
        m_OnClicked();
    }
}
