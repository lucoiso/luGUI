// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include <imgui.h>

module luGUI.UserInterface.Panels.Grid;

using namespace luGUI;

void Grid::Draw()
{
    if (std::empty(m_Items))
    {
        return;
    }

    m_NumColumns = 0;
    for (const auto &LineIt : m_Items)
    {
        if (std::size_t const CurrentSize = std::size(LineIt);
            CurrentSize > m_NumColumns)
        {
            m_NumColumns = static_cast<std::int32_t>(CurrentSize);
        }
    }

    SetupAlignment();
    Render();
}

void Grid::Render()
{
    m_ItemSize                   = { 0.F, 0.F };
    bool const  UsingCustomWidth = HasCustomWidth();
    float const WidthPerItem     = GetWidth() / static_cast<float>(m_NumColumns);

    ImGui::BeginTable("##GenericGridView", m_NumColumns);

    auto const Flags = HasCustomWidth() ? ImGuiTableColumnFlags_WidthFixed : ImGuiTableColumnFlags_WidthStretch;
    ImGui::TableSetupColumn(nullptr, Flags, WidthPerItem);

    for (const auto &LineIt : m_Items)
    {
        ImGui::TableNextRow();
        for (const auto &ColumnIt : LineIt)
        {
            ImGui::TableNextColumn();

            if (UsingCustomWidth)
            {
                ColumnIt->SetWidth(WidthPerItem);
            }

            ColumnIt->Draw();

            ImVec2 const &ItemItSize = ColumnIt->GetItemSize();

            m_ItemSize.x = ItemItSize.x > m_ItemSize.x ? ItemItSize.x : m_ItemSize.x;
            m_ItemSize.y = ItemItSize.y > m_ItemSize.y ? ItemItSize.y : m_ItemSize.y;
        }
    }

    m_ItemSize.x *= static_cast<float>(m_NumColumns);
    m_ItemSize.y *= static_cast<float>(std::size(m_Items));

    ImGui::EndTable();
}
