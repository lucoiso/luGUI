// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include <imgui.h>

module luGUI.UserInterface.Items.Image;

import luGUI.UserInterface.Singletons.ImageManager;

using namespace luGUI;

Image::Image(float const Width)
    : Item(Width)
{
}

Image::Image(strzilla::string_view const &Key, float const Width)
    : Item(Width)
  , m_Key(Key)
{
}

Image::Image(strzilla::string_view const &Key, ImageDefinitions &&Definitions, float const Width)
    : Item(Width)
  , m_Key(Key)
  , m_Definitions(Definitions)
{
}

void Image::Render()
{
    VkDescriptorSet const ImageDescriptor = ImageManager::Get().GetDescriptor(m_Key);
    if (ImageDescriptor == VK_NULL_HANDLE)
    {
        return;
    }

    ImGui::Image(ImageDescriptor, m_Definitions.Size, m_Definitions.UV0, m_Definitions.UV1, m_Definitions.ImageColor, m_Definitions.BorderColor);
}
