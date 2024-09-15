// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

module luGUI.UserInterface.Panels.Stack;

using namespace luGUI;

Stack::Stack(Orientation const Orientation)
    : m_Orientation(Orientation)
{
}

Stack::Stack(std::vector<std::shared_ptr<Item>> &&Items)
    : m_Items(Items)
{
}

Stack::Stack(Orientation const Orientation, std::vector<std::shared_ptr<Item>> &&Items)
    : m_Orientation(Orientation)
  , m_Items(Items)
{
}

void Stack::Draw()
{
    if (std::empty(m_Items))
    {
        return;
    }

    switch (m_Orientation)
    {
        case Orientation::Horizontal:
        {
            for (std::size_t Iterator = 0; Iterator < std::size(m_Items); ++Iterator)
            {
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
                ItemIt->Draw();
            }

            ImGui::EndGroup();
            break;
        }
        default:
            std::unreachable();
    }
}
