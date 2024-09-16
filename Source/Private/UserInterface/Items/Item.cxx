// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include <imgui.h>

module luGUI.UserInterface.Items.Item;

using namespace luGUI;

Item::Item(float Width)
    : m_Width(Width)
{
}

void Item::Draw()
{
    bool const PushWidth = HasCustomWidth();

    if (PushWidth)
    {
        ImGui::PushItemWidth(m_Width);
    }

    Render();

    if (PushWidth)
    {
        ImGui::PopItemWidth();
    }
}
