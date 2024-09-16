// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include <imgui.h>
#include <imgui_internal.h>

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

    Render();
}

void Grid::Render()
{
    ImGui::BeginTable("##GenericGridView", m_NumColumns);

    auto const Flags = HasCustomWidth() ? ImGuiTableColumnFlags_WidthFixed : ImGuiTableColumnFlags_WidthStretch;
    ImGui::TableSetupColumn(nullptr, Flags, m_Width / static_cast<float>(m_NumColumns));

    for (const auto &LineIt : m_Items)
    {
        ImGui::TableNextRow();
        for (const auto &ColumnIt : LineIt)
        {
            ImGui::TableNextColumn();
            ColumnIt->Draw();
        }
    }

    ImGui::EndTable();
}
