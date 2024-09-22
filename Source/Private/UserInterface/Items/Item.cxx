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

    SetupItemAlignment();
    Render();

    m_ItemSize = ImGui::GetItemRectSize();

    if (PushWidth)
    {
        ImGui::PopItemWidth();
    }
}

void Item::SetupItemAlignment()
{
    if (m_Alignment == Alignment::None)
    {
        return;
    }

    m_ItemSize.x = std::truncf(m_ItemSize.x * 1.13F); // TODO : Fix incorrect offset
    m_ItemSize.y = std::truncf(m_ItemSize.y * 1.20F); // TODO : Fix incorrect offset

    ImVec2 const AvailableSpace = ImGui::GetContentRegionAvail();
    ImVec2 AlignmentPosition;

    switch (m_Alignment)
    {
        case Alignment::Left:
        {
            AlignmentPosition = ImVec2 { 0.F, (AvailableSpace.y - m_ItemSize.y) * 0.5F };
            break;
        }
        case Alignment::TopLeft:
        {
            AlignmentPosition = ImVec2 { 0.F, 0.F };
            break;
        }
        case Alignment::Top:
        {
            AlignmentPosition = ImVec2 { (AvailableSpace.x - m_ItemSize.x) * 0.5F, 0 };
            break;
        }
        case Alignment::TopRight:
        {
            AlignmentPosition = ImVec2 { AvailableSpace.x - m_ItemSize.x, 0.F };
            break;
        }
        case Alignment::Right:
        {
            AlignmentPosition = ImVec2 { AvailableSpace.x - m_ItemSize.x, (AvailableSpace.y - m_ItemSize.y) * 0.5F };
            break;
        }
        case Alignment::BottomLeft:
        {
            AlignmentPosition = ImVec2 { 0.F, AvailableSpace.y - m_ItemSize.y };
            break;
        }
        case Alignment::Bottom:
        {
            AlignmentPosition = ImVec2 { (AvailableSpace.x - m_ItemSize.x) * 0.5F, AvailableSpace.y - m_ItemSize.y };
            break;
        }
        case Alignment::BottomRight:
        {
            AlignmentPosition = ImVec2 { AvailableSpace.x - m_ItemSize.x, AvailableSpace.y - m_ItemSize.y };
            break;
        }
        case Alignment::Center:
        {
            AlignmentPosition = ImVec2 { (AvailableSpace.x - m_ItemSize.x) * 0.5F, (AvailableSpace.y - m_ItemSize.y) * 0.5F };
            break;
        }
        case Alignment::None: default: std::unreachable();
    }

    ImGui::SetCursorPos(AlignmentPosition);
}
