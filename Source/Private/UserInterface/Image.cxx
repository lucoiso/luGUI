// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

module luGUI.UserInterface.Image;

// TODO : Implement class

using namespace luGUI;

Image::Image(Control *const Parent)
    : Control(Parent)
{
}

Image::~Image()
{
    //if (m_DescriptorSet != VK_NULL_HANDLE)
    //{
    //    ImGuiVulkanRemoveTexture(m_DescriptorSet);
    //}
}

void Image::Refresh()
{
    //if (m_DescriptorSet != VK_NULL_HANDLE)
    //{
    //    ImGuiVulkanRemoveTexture(m_DescriptorSet);
    //}

    //VkSampler const Sampler { Renderer::GetSampler() };
    //VkImageView const View = GetImageDescriptor().imageView;

    //if (Sampler != VK_NULL_HANDLE && View != VK_NULL_HANDLE)
    //{
    //    m_DescriptorSet = ImGuiVulkanAddTexture(Sampler, View, g_ReadLayout);
    //}
}

void Image::PrePaint()
{
}

void Image::Paint()
{
}

void Image::PostPaint()
{
}
