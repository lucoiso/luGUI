// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include <imgui.h>

module luGUI.UserInterface.Items.Item;

using namespace luGUI;

Item::Item(float const Width, Alignment const Alignment)
    : m_Width(Width)
  , m_Alignment(Alignment)
{
}

void Item::Draw()
{
    bool const PushWidth = HasCustomWidth();

    if (PushWidth)
    {
        ImGui::PushItemWidth(m_Width);
    }

    // TODO : Align according to m_Alignment

    Render();

    if (PushWidth)
    {
        ImGui::PopItemWidth();
    }
}
