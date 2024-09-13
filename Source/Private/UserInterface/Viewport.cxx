// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

module luGUI.UserInterface.Viewport;

import RenderCore.Renderer;
import RenderCore.Runtime.Scene;
import RenderCore.Utils.Constants;
import luGUI.Integrations.GLFWCallbacks;
import luGUI.Integrations.ImGuiVulkanBackend;

using namespace luGUI;

Viewport::Viewport(Control *const Parent)
    : Control(Parent)
{
    RenderCore::Renderer::SetRenderOffscreen(true);
    SetViewportControlsCamera(true);
}

Viewport::~Viewport()
{
    if (!std::empty(m_ViewportDescriptorSets))
    {
        for (auto const &DescriptorSetIter : m_ViewportDescriptorSets)
        {
            if (DescriptorSetIter != VK_NULL_HANDLE)
            {
                ImGuiVulkanRemoveTexture(DescriptorSetIter);
            }
        }
        m_ViewportDescriptorSets.clear();
    }
}

void Viewport::Refresh()
{
    if (!std::empty(m_ViewportDescriptorSets))
    {
        for (auto const &DescriptorSetIter : m_ViewportDescriptorSets)
        {
            if (DescriptorSetIter != VK_NULL_HANDLE)
            {
                ImGuiVulkanRemoveTexture(DescriptorSetIter);
            }
        }
        m_ViewportDescriptorSets.clear();
    }

    VkSampler const Sampler { RenderCore::GetSampler() };

    if (std::vector const ImageViews { RenderCore::Renderer::GetOffscreenImages() };
        Sampler != VK_NULL_HANDLE && !std::empty(ImageViews))
    {
        for (auto const &ImageViewIter : ImageViews)
        {
            m_ViewportDescriptorSets.push_back(ImGuiVulkanAddTexture(Sampler, ImageViewIter, RenderCore::g_ReadLayout));
        }
    }
}

void Viewport::PrePaint()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4 { 0.F, 0.F, 0.F, 1.F });

    constexpr auto Flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration;
    m_Open = ImGui::Begin("Viewport", nullptr, Flags) && ImGui::IsItemVisible();

    ImGui::PopStyleColor();
    SetViewportHovering(ImGui::IsWindowHovered());
}

void Viewport::Paint()
{
    if (m_Open && !std::empty(m_ViewportDescriptorSets))
    {
        if (std::optional<std::int32_t> const &ImageIndex = RenderCore::Renderer::GetImageIndex();
            ImageIndex.has_value())
        {
            ImVec2 const ViewportSize { ImGui::GetContentRegionAvail() };
            ImGui::Image(m_ViewportDescriptorSets.at(ImageIndex.value()), ImVec2 { ViewportSize.x, ViewportSize.y });
        }
    }
}

void Viewport::PostPaint()
{
    ImGui::End();
}
