// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include <imgui.h>

module luGUI.UserInterface.Singletons.FontManager;

import luGUI.Integrations.ImGuiVulkanBackend;

using namespace luGUI;

FontManager &FontManager::Get()
{
    static FontManager Singleton {};
    return Singleton;
}

bool FontManager::RegisterFont(const char* const Key, strzilla::string_view const &Path, float const Pixels)
{
    if (!Key || std::empty(Path) || Pixels <= 0.F)
    {
        return false;
    }

    auto const Hash = m_HashCreator(Key);
    if (m_Fonts.contains(Hash))
    {
        return false;
    }

    ImGuiIO const &IO      = ImGui::GetIO();
    ImFont *       NewFont = IO.Fonts->AddFontFromFileTTF(std::data(Path), Pixels);

    if (!NewFont)
    {
        return false;
    }

    m_Fonts.emplace(Hash, NewFont);
    return true;
}

void FontManager::BuildFonts()
{
    ImGuiVulkanDestroyFontsTexture();

    ImGuiIO const &IO = ImGui::GetIO();
    IO.Fonts->Build();

    ImGuiVulkanCreateFontsTexture();
}

void FontManager::UnregisterFont(const char* const Key)
{
    if (auto const Hash = m_HashCreator(Key);
        m_Fonts.contains(Hash))
    {
        m_Fonts.erase(Hash);
    }
}

void FontManager::UnregisterAllFonts()
{
    if (std::empty(m_Fonts))
    {
        return;
    }

    if (ImGuiIO const &IO = ImGui::GetIO();
        IO.Ctx != nullptr)
    {
        IO.Fonts->ClearFonts();
        m_Fonts.clear();
    }
}
