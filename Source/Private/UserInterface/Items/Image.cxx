// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include <imgui.h>

module luGUI.UserInterface.Items.Image;

import luGUI.UserInterface.Singletons.ImageManager;

using namespace luGUI;

void Image::Render()
{
    VkDescriptorSet const ImageDescriptor = ImageManager::Get().GetDescriptor(std::data(m_Key));
    if (ImageDescriptor == VK_NULL_HANDLE)
    {
        return;
    }

    ImGui::Image(ImageDescriptor, m_Definitions.Size, m_Definitions.UV0, m_Definitions.UV1, m_Definitions.ImageColor, m_Definitions.BorderColor);
}
