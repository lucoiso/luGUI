// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

module luGUI.UserInterface.Items.Button;

using namespace luGUI;

Button::Button(strzilla::string_view const &Label)
    : m_Label(Label)
{
}

Button::Button(strzilla::string_view const &Label, std::function<void()> &&OnClicked)
    : m_Label(Label)
  , m_OnClicked(OnClicked)
{
}

void Button::Draw()
{
    if (ImGui::Button(std::data(m_Label)))
    {
        m_OnClicked();
    }
}
