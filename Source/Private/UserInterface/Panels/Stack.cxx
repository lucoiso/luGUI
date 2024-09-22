// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include <imgui.h>

module luGUI.UserInterface.Panels.Stack;

using namespace luGUI;

void Stack::Draw()
{
    if (std::empty(m_Items))
    {
        return;
    }

    SetupAlignment();
    Render();
}

void Stack::Render()
{
    m_ItemSize                  = { 0.F, 0.F };
    bool const UsingCustomWidth = HasCustomWidth();

    switch (m_Orientation)
    {
        case Orientation::Horizontal:
        {
            auto const SizePerItem = GetWidth() / static_cast<float>(std::size(m_Items));

            for (std::size_t Iterator = 0; Iterator < std::size(m_Items); ++Iterator)
            {
                auto const &ItemIt = m_Items.at(Iterator);
                if (UsingCustomWidth)
                {
                    ItemIt->SetWidth(SizePerItem);
                }

                ItemIt->Draw();

                ImVec2 const &ItemItSize = ItemIt->GetItemSize();

                m_ItemSize.x += ItemItSize.x;
                m_ItemSize.y = ItemItSize.y > m_ItemSize.y ? ItemItSize.y : m_ItemSize.y;

                if (Iterator < std::size(m_Items) - 1)
                {
                    ImGui::SameLine();
                }
            }
            break;
        }
        case Orientation::Vertical:
        {
            ImGui::BeginGroup();

            for (std::shared_ptr<Item> const &ItemIt : m_Items)
            {
                if (UsingCustomWidth)
                {
                    ItemIt->SetWidth(GetWidth());
                }

                ItemIt->Draw();

                ImVec2 const &ItemItSize = ItemIt->GetItemSize();

                m_ItemSize.x = ItemItSize.x > m_ItemSize.x ? ItemItSize.x : m_ItemSize.x;
                m_ItemSize.y += ItemItSize.y;
            }

            ImGui::EndGroup();
            break;
        }
        default:
            std::unreachable();
    }
}
