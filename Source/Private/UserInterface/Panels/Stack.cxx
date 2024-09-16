// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include <imgui.h>
#include <imgui_internal.h>

module luGUI.UserInterface.Panels.Stack;

using namespace luGUI;

Stack::Stack(float const Width)
    : Item(Width)
{
}

Stack::Stack(Orientation const Orientation, float const Width)
    : Item(Width)
  , m_Orientation(Orientation)
{
}

Stack::Stack(std::vector<std::shared_ptr<Item>> &&Items, float const Width)
    : Item(Width)
  , m_Items(Items)
{
}

Stack::Stack(Orientation const Orientation, std::vector<std::shared_ptr<Item>> &&Items, float const Width)
    : Item(Width)
  , m_Orientation(Orientation)
  , m_Items(Items)
{
}

void Stack::Draw()
{
    if (std::empty(m_Items))
    {
        return;
    }

    if (HasCustomWidth())
    {
        ImGui::PushMultiItemsWidths(static_cast<std::int32_t>(std::size(m_Items)), m_Width);
    }

    Render();
}

void Stack::Render()
{
    bool const UsingCustomWidth = HasCustomWidth();

    switch (m_Orientation)
    {
        case Orientation::Horizontal:
        {
            auto const SizePerItem = m_Width / static_cast<float>(std::size(m_Items));

            for (std::size_t Iterator = 0; Iterator < std::size(m_Items); ++Iterator)
            {
                if (UsingCustomWidth)
                {
                    m_Items.at(Iterator)->SetWidth(SizePerItem);
                }

                m_Items.at(Iterator)->Draw();

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
                    ItemIt->SetWidth(m_Width);
                }

                ItemIt->Draw();
            }

            ImGui::EndGroup();
            break;
        }
        default:
            std::unreachable();
    }
}
