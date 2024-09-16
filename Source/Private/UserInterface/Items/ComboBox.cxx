// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include <imgui.h>

module luGUI.UserInterface.Items.ComboBox;

using namespace luGUI;

ComboBox::ComboBox(float const Width)
    : Item(Width)
{
}

ComboBox::ComboBox(strzilla::string_view const &Label, float const Width)
    : Item(Width)
  , m_Label(Label)
{
    UpdateInternalOptions();
}

ComboBox::ComboBox(strzilla::string_view const &Label, std::vector<strzilla::string> *const Options, float const Width)
    : Item(Width)
  , m_Label(Label)
  , m_Options(Options)
{
    UpdateInternalOptions();
}

ComboBox::ComboBox(strzilla::string_view const &Label, std::function<void(strzilla::string)> &&OnChanged, float const Width)
    : Item(Width)
  , m_Label(Label)
  , m_OnChanged(OnChanged)
{
    SetWidth(Width);
    UpdateInternalOptions();
}

ComboBox::ComboBox(strzilla::string_view const &           Label,
                   std::vector<strzilla::string> *const    Options,
                   std::function<void(strzilla::string)> &&OnChanged,
                   float const                             Width)
    : Item(Width)
  , m_Label(Label)
  , m_Options(Options)
  , m_OnChanged(OnChanged)
{
    UpdateInternalOptions();
}

void ComboBox::Render()
{
    if (!m_Options || std::empty(*m_Options))
    {
        return;
    }

    CheckOptions();

    if (ImGui::Combo("##SetAsChildOfComboBox",
                     &m_CurrentSelection,
                     std::data(m_InternalOptions),
                     static_cast<std::int32_t>(std::size(m_InternalOptions))) && m_OnChanged)
    {
        m_OnChanged(m_Options->at(m_CurrentSelection));
    }
}

void ComboBox::CheckOptions()
{
    if (m_Options == nullptr || std::empty(*m_Options))
    {
        if (!std::empty(m_InternalOptions))
        {
            m_InternalOptions.clear();
        }

        return;
    }

    if (std::size(*m_Options) != std::size(m_InternalOptions))
    {
        UpdateInternalOptions();
        return;
    }

    for (std::size_t Iterator = 0; Iterator < std::size(m_InternalOptions); ++Iterator)
    {
        if (m_Options->at(Iterator) != m_InternalOptions.at(Iterator))
        {
            UpdateInternalOptions();
            return;
        }
    }
}

void ComboBox::UpdateInternalOptions()
{
    if (!m_Options || std::empty(*m_Options))
    {
        if (!std::empty(m_InternalOptions))
        {
            m_InternalOptions.clear();
        }

        return;
    }

    m_InternalOptions.resize(std::size(*m_Options));

    for (std::size_t Iterator = 0; Iterator < std::size(m_InternalOptions); ++Iterator)
    {
        m_InternalOptions.at(Iterator) = std::data(m_Options->at(Iterator));
    }
}
