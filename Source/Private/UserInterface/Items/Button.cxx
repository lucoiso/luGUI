// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include <imgui.h>

module luGUI.UserInterface.Items.Button;

using namespace luGUI;

Button::Button(float const Width)
    : Item(Width)
{
}

Button::Button(strzilla::string_view const &Label, float const Width)
    : Item(Width)
  , m_Label(Label)
{
}

Button::Button(strzilla::string_view const &Label, std::function<void()> &&OnClicked, float const Width)
    : Item(Width)
  , m_Label(Label)
  , m_OnClicked(OnClicked)
{
}

void Button::Render()
{
    if (ImGui::Button(std::data(m_Label)) && m_OnClicked)
    {
        m_OnClicked();
    }
}
