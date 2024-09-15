// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include <imgui.h>

module luGUI.UserInterface.Panels.Grid;

using namespace luGUI;

Grid::Grid(std::vector<std::vector<std::shared_ptr<Item>>> &&Items)
    : m_Items(Items)
{
}

void Grid::Draw()
{
    if (std::empty(m_Items))
    {
        return;
    }

    std::size_t MaxColumns = 0;
    for (const auto &LineIt : m_Items)
    {
        if (std::size_t const CurrentSize = std::size(LineIt);
            CurrentSize > MaxColumns)
        {
            MaxColumns = CurrentSize;
        }
    }

    ImGui::BeginTable("##GenericGridView", static_cast<std::int32_t>(MaxColumns));

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
